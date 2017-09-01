#include "easyos.h"

/*
*********************************************************************************************************
*                                           SEMPEND
*
* Description: This function will pend current task for timeout times
*
* Arguments  : event          pending on event struct
*              timeout        pending time
*
* Returns    : 0 success  others fail
*********************************************************************************************************
*/
U8  SemPend(OSEVENT *event,U32 timeout)
{
	if(event == NULL || event->eventType != SEM_EVENT)
	{
		return ERR_TYPE;
	}
	SetRdyPrio(current->prio,&event->eventGrp,&event->eventRdy[0]);
	ClearRdyPrio(current->prio,&RdyGroup,&RdyTbl[0]);
	OsSched();
	return 0;
}
/*
*********************************************************************************************************
*                                           SEMPOST
*
* Description: This function will post sem to wakeup pending task
*
* Arguments  : event          wakeup  event struct
*
*
* Returns    : 0 success  others fail
*********************************************************************************************************
*/
U8  SemPost(OSEVENT *event)
{
	U32 prio;
	if(event == NULL || event->eventType != SEM_EVENT)
	{
		return ERR_TYPE;
	}
	prio = GetRdyHighPrio(&event->eventGrp,&event->eventRdy[0]);
	ClearRdyPrio(prio,&event->eventGrp,&event->eventRdy[0]);
	SetRdyPrio(prio,&RdyGroup,&RdyTbl[0]);
	return 0;
}

/*
*********************************************************************************************************
*                                           QUEUEPEND
*
* Description: This function will pend current task to waiting queue message
*
* Arguments  : event          pending on event struct
               timeout        pending time
*
* Returns    : 0 success  others fail
*********************************************************************************************************
*/
U8  QueuePend(OSEVENT *event,U32 timeout)
{
	if(event == NULL || event->eventType != QUEUE_EVENT)
	{
		return ERR_TYPE;
	}
	SetRdyPrio(current->prio,&event->eventGrp,&event->eventRdy[0]);
	ClearRdyPrio(current->prio,&RdyGroup,&RdyTbl[0]);
	OsSched();
	return 0;
}
/*
*********************************************************************************************************
*                                           QUEUEPOST
*
* Description: This function will post queue message to waiting task
*
* Arguments  : event          queue event struct
               buffer         copy queue message to buffer
*
* Returns    : 0 success  others fail
*********************************************************************************************************
*/
U8  QueuePost(OSEVENT *event,U8 *buffer)
{
	U32 prio;
	if(event == NULL || event->eventType != QUEUE_EVENT)
	{
		return ERR_TYPE;
	}
	if(buffer == NULL)
	{
		return ERR_PRT;
	}
	prio = GetRdyHighPrio(&event->eventGrp,&event->eventRdy[0]);
	ClearRdyPrio(prio,&event->eventGrp,&event->eventRdy[0]);
	osmemcpy(buffer,event->msg,event->msgLenth);
	SetRdyPrio(prio,&RdyGroup,&RdyTbl[0]);
	return 0;
}
