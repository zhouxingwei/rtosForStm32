#include "easyos.h"


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

