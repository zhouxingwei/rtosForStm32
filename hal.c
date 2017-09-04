#include "easyos.h"
#include "cm3_hal.h" 
#include "sys.h"


__asm void SVC_Handler(void)
{
	extern 	 current;
	PRESERVE8
	ldr	r3, =current		/* Restore the context. */
	ldr r1, [r3]				/* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
	ldr r0, [r1]				/* The first item in pxCurrentTCB is the task top of stack. */
	ldmia r0!, {r4-r11}		/* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
	msr psp, r0					/* Restore the task stack pointer. */
	orr r14, #0xd             /*return thread mode,used psp and thumb instruction*/				
	bx r14	
	nop
}
__asm void PendSV_Handler(void)
{
	extern 	 current;
	extern   pLastTCB;
	PRESERVE8
	mrs r0,psp
	ldr	r3, =pLastTCB
	ldr r2, [r3]				/* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
	stmdb r0!,{r4-r11}
	str r0, [r2]
	ldr	r3, =current
	ldr r1, [r3]					 
	ldr r0, [r1]					/* The first item in pxCurrentTCB is the task top of stack. */
	ldmia r0!, {r4-r11}		/* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
	msr psp, r0					/* Restore the task stack pointer. */	
	bx r14
}

void SysTick_Handler(void)
{
	U32 i = 0;
	osGlobal.systemTick++;
	if(osGlobal.timerNum > 0)
	{
		for(i=0;i<TIMER_MAX_NUM;i++)
		{
			if(timerlist[i].timerId > 0)   //timer not used
			{
				if(timerlist[i].tickLeft > 0)
				{
					timerlist[i].tickLeft--;
				}
				else if(timerlist[i].tickLeft == 0)
				{
					SetRdyPrio(timerlist[i].prio,&RdyGroup,&RdyTbl[0]);
					delTimerFromList(timerlist[i].prio);
					continue;
				}
			}
		}
	}
	OsSched();
}
/*------------------------------------------------------------
                  外部8M,则得到72M的系统时钟
------------------------------------------------------------*/	
void Stm32_Clock_Init(void)
{
	unsigned char temp=0;
 	U8 timeout=0;
	RCC_DeInit();
	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON

	timeout=0;
	while(!(RCC->CR>>17)&&timeout<200)timeout++;//等待外部时钟就绪	 

	//0-24M 等待0;24-48M 等待1;48-72M等待2;(非常重要!)	   
	FLASH->ACR|=0x32;//FLASH 2个延时周期

	RCC->CFGR|=0X001D2400;//APB1/2=DIV2;AHB=DIV1;PLL=9*CLK;HSE作为PLL时钟源
	RCC->CR|=0x01000000;  //PLLON

	timeout=0;
	while(!(RCC->CR>>25)&&timeout<200)timeout++;//等待PLL锁定

	RCC->CFGR|=0x00000002;//PLL作为系统时钟
	while(temp!=0x02&&timeout<200)     //等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		timeout++;
		temp&=0x03;
	}  
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->LOAD = (CONFIG_CLOCK_TICK) - 1;
	SysTick->VAL = CONFIG_CLOCK_TICK;
	SysTick->CTRL = (1 << 2) | (1<<0) | (1<<1);//使能IRQ（

}

__asm void distableInterrupt( void )
{
	PRESERVE8
	CPSID  I
	//push { r0 }
	//mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
	//msr basepri, r0
	//pop { r0 }
	//bx r14
}

/*-----------------------------------------------------------*/

__asm void entableInterrupt( void )
{
	PRESERVE8
	CPSIE  I
	//push { r0 }
	//mov r0, #0
	//msr basepri, r0
	//pop { r0 }
	//bx r14
}
