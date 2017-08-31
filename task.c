#include <stdio.h>

#include "easyos.h"

U8 RdyTbl[4]; //16 prorioty
U8 RdyGroup = 0;
OSTCB taskTbl[MAX_PRIO];
OSTCB *current;
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

void OsSched(void)
{
	U32 i;

	//current = &taskTbl[i];
}

void TaskDelay(U32 delay)
{

	OsSched();
}


int main(void) //for test
{
	U32 prio,testprio;
	DP("enter test prio:\n");
	scanf("%d",&testprio);
	SetRdyPrio(testprio,&RdyGroup,&RdyTbl[0]);
	scanf("%d",&testprio);
	SetRdyPrio(testprio,&RdyGroup,&RdyTbl[0]);
	scanf("%d",&testprio);
	SetRdyPrio(testprio,&RdyGroup,&RdyTbl[0]);
	prio = GetRdyHighPrio(&RdyGroup,&RdyTbl[0]);
	DP("get highest prio is %d\n",prio);

	DP("rdytbl[0]:%d,1:%d,2:%d,3:%d,group:%d\n",RdyTbl[0],RdyTbl[1],RdyTbl[2],RdyTbl[3],RdyGroup);
	ClearRdyPrio(prio,&RdyGroup,&RdyTbl[0]);
	DP("clear,rdytbl[0]:%d,1:%d,2:%d,3:%d,group is %d\n",RdyTbl[0],RdyTbl[1],RdyTbl[2],RdyTbl[3],RdyGroup);
	return 0;
}
