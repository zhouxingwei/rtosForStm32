/**
  ******************************************************************************
  * @file    
  * @author  zhouxingwei
  * @version V0.0.1
  * @date    08-July-2017
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CM3_HAL_H
#define __STM32F10x_CM3_HAL_H

#define CLOCK 72/8 //时钟=72M
#define CONFIG_CLOCK_TICK 720000        //10ms
#define TIME_OVERFLOW  100*3600*24*7    //time overflow

#define NVIC_SYSTICK_CTRL		(( volatile unsigned long *)0xe000e010)
#define NVIC_SYSTICK_LOAD		(( volatile unsigned long *)0xe000e014)
#define NVIC_INT_CTRL			(( volatile unsigned long *)0xe000ed04)
#define NVIC_SYSPRI2			(( volatile unsigned long *)0xe000ed20)
#define NVIC_SYSTICK_CLK		0x00000004
#define NVIC_SYSTICK_INT		0x00000002
#define NVIC_SYSTICK_ENABLE		0x00000001
#define NVIC_PENDSVSET			0x10000000
#define NVIC_PENDSV_PRI			( ( ( unsigned long ) 255 ) << 16 )
#define NVIC_SYSTICK_PRI		( ( ( unsigned long ) 255 ) << 24 )
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void Stm32_Clock_Init(void);
#endif
