#include <stdio.h>

#include "easyos.h"

OSCONTROL osGlobal;
OSTIME timerlist[TIMER_MAX_NUM];
OSTCB taskTbl[MAX_PRIO];

U8 RdyTbl[4]; //16 prorioty
U8 RdyGroup = 0;
OSTCB * volitile current;
OSTCB * volatile pLastTCB;
static U8 UnPrioMap[16]={0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3};
/*
*********************************************************************************************************
*                                           GET HIGHEST READY PRIORITY
*
* Description: This function get highest priority.
*
* Arguments  : void
*
* Returns    : highest ready priority
*********************************************************************************************************
*/
U32 GetRdyHighPrio(U8 *group,U8 *RdyTbl)
{
	U32 UnY = 0;
	if(*group > MAX_PRIO-1 || *group == 0)
	{
		DP("RdyGroup is not right\n");
		return -1;
	}
	UnY = UnPrioMap[*group];
	return ((UnY<<2)+UnPrioMap[RdyTbl[UnY]]);
}
/*
*********************************************************************************************************
*                                           SET READY PRIORITY
*
* Description: This function creates a semaphore.
*
* Arguments  : prio           is will be set priority value,from 0-15
*
* Returns    void
*********************************************************************************************************
*/
void SetRdyPrio(U32 prio,U8 *group,U8 *RdyTbl)
{
	if(prio > 15)
	{
		return;
	}
	*group |= (1<<(prio>>2));
	RdyTbl[prio>>2] |= (1<<(prio%4));
}
/*
*********************************************************************************************************
*                                           CLEAR READY PRIORITY
*
* Description: This function clear unvalid priority from ready table.
*
* Arguments  : prio          is the prio value will be clear from RdyTbl.
*
* Returns    : void
*********************************************************************************************************
*/
void ClearRdyPrio(U32 prio,U8 *group,U8 *RdyTbl)
{
	RdyTbl[prio>>2] &= ~(1<<(prio%4));
	if(RdyTbl[prio>>2] == 0)      /*=================rdy group is all zero,should clear group bit*/
	{
		*group &= ~(1<<(prio>>2));
	}
}
/*
*********************************************************************************************************
*                                           OS SCHEDULE TASK
*
* Description: This function switch task form current to highest priority ready task
*
* Arguments  : void
*
* Returns    : void
*********************************************************************************************************
*/
void OsSched(void)
{
	U32 i,highestprio;
	highestprio = GetRdyHighPrio(&RdyGroup,&RdyTbl[0]);
	pLastTCB = current;
	current = &taskTbl[highestprio];
	if(current != pLastTCB)
	{
		*NVIC_INT_CTRL = NVIC_PENDSVSET;    //set schedule to switch new task
	}
}
/*
*********************************************************************************************************
*                                           ADD NEW DALAY TIMER TO TIMERLIST
*
* Description: This function add new timer to timerlist array
*
* Arguments  : prio          is the prio value will be delayed
			   delay         is the delayed time
*
* Returns    : 1----fail,0----success
*********************************************************************************************************
*/
U32 addTimerToList(U8 prio,U32 delay)
{
	U32 ret = 1,i = 0;
	for(i=0;i<TIMER_MAX_NUM;i++)
	{
		if(timerlist[i].timerId == 0)   //timer not used
		{
			osGlobal.timerNum++;
			timerlist[i].timerId = osGlobal.timerNum;
			timerlist[i].prio = prio;
			timerlist[i].tickLeft = delay;   //10ms*delay
			ret = 0;
			break;
		}
	}
	return ret;
}
/*
*********************************************************************************************************
*                                           DELLETE  DALAY TIMER FROM TIMERLIST
*
* Description: This function delete old timer from timerlist array
*
* Arguments  : prio          is the prio value will be delayed
*
* Returns    : 1----fail,0----success
*********************************************************************************************************
*/
U32 delTimerFromList(U8 prio)
{
	U32 ret = 1,i = 0;
	for(i=0;i<TIMER_MAX_NUM;i++)
	{
		if(timerlist[i].prio == prio)   //timer not used
		{
			osGlobal.timerNum--;
			osmemset(&timerlist[i],0,sizeof(OSTIME));
			ret = 0;
			break;
		}
	}
	return ret;
}
/*
*********************************************************************************************************
*                                           TASK DELAY
*
* Description: This function will suspend current task delay*10ms
*
* Arguments  : delay          current task will be delay,time is delay*10ms
*
* Returns    : void
*********************************************************************************************************
*/
void TaskDelay(U32 delay)
{
	addTimerToList(current->prio,delay);
	ClearRdyPrio(current->prio,&RdyGroup,&RdyTbl[0]);
	OsSched();
}
/*
*********************************************************************************************************
*                                           SYSTEM INIT
*
* Description: Init system
*
* Arguments  : void
*
* Returns    : void
*********************************************************************************************************
*/
void SystemInit(void)
{
	osGlobal.timerNum = 0;
	osGlobal.scheduleType = RTT_SCHDULE;
}
/*
*********************************************************************************************************
*                                           SYSTEM INIT
*
* Description: SCHEDULE FOR FIRST
* Arguments  : void
*
* Returns    : void
*********************************************************************************************************
*/
ULONG * init_stack(ULONG *stack,void (*task_func)(void))
{

	/* Simulate the stack frame as it would be created by a context switch
	interrupt. */
	*stack = INIT_PSR_VALUE;	/* xPSR */
	stack--;
	*stack = (ULONG)task_func;	/* PC */
	stack--;
	*stack = (ULONG)task_func;	 /* LR */
	stack -= 5;	 /* R12, R3, R2 and R1. */
	*stack = 0; 	/* R0 */
	stack -= 8;	/* R11, R10, R9, R8, R7, R6, R5 and R4. */

	return stack;
}
/*
*********************************************************************************************************
*                                           SCHEDULE
*
* Description: SCHEDULE FOR FIRST
* Arguments  : void
*
* Returns    : void
*********************************************************************************************************
*/
__asm void Schedule(void)
{
//	extern easyTask_timeDisplay;
	/* Use the NVIC offset register to locate the stack. */
	ldr r0, =0xE000ED08
	ldr r0, [r0]
	ldr r0, [r0]
	/* Set the msp back to the start of the stack. */
	msr msp, r0
	/* Call SVC to start the first task. */
	svc 0
//	b easyTask_timeDisplay
	ALIGN
	
}
/*
*********************************************************************************************************
*                                           STARTSCHDULE
*
* Description: STARTSCHDULE
*                                     
* Arguments  : void
*
* Returns    : void
*********************************************************************************************************
*/
void startSchedule(void)
{
	Schedule();
}
/*
*********************************************************************************************************
*                                           TaskCreate
*
* Description: STARTSCHDULE
*
* Arguments  : void
*
* Returns    : void
*********************************************************************************************************
*/
void  taskCreate(void (*func)(void), ULONG *stack, U8 taskId, U8 priority)
{
	U32 highprio = 0;
	taskTbl[priority].pStack = init_stack(stack,func);
	taskTbl[priority].prio = priority;
	taskTbl[priority].taskId = taskId;
	taskTbl[priority].taskState = TASK_READY;
	taskTbl[priority].pendState = NOT_PEND;
	SetRdyPrio(priority,&RdyGroup,&RdyTbl[0]);
	highprio = GetRdyHighPrio(&RdyGroup,&RdyTbl[0]);
	current = &taskTbl[highprio];
}