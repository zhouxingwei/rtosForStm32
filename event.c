#include "easyos.h"
OSEVENT eventlist[EVENT_MAX_NUM];
/*
*********************************************************************************************************
*                                           SEMINIT
*
* Description: This function will init sem event
*
* Arguments  : void
*
* Returns    : fail---null,success---sem event pointer
*********************************************************************************************************
*/
OSEVENT * SemInit(void)
{
	OSEVENT *semptr = NULL;
	INT16 i=0;
	for(i=0;i<EVENT_MAX_NUM;i++)
	{
		if(eventlist[i].eventType == INIT_EVENT)
		{
			eventlist[i].eventType = SEM_EVENT;
			eventlist[i].msgLenth = 0;
			eventlist[i].msg = NULL;			
			semptr = &eventlist[i];
			break;
		}
	}
	return semptr;
}
/*
*********************************************************************************************************
*                                           DELSEM
*
* Description: This function will delete sem event
*
* Arguments  : OSEVENT of sem will be delete
*
* Returns    : fail----1,success---sem event pointer
*********************************************************************************************************
*/
INT8 SemDel(OSEVENT *semptr)
{
	INT8 ret = -1;
	INT16 i=0;
	for(i=0;i<EVENT_MAX_NUM;i++)
	{
		if(semptr == &eventlist[i])
		{
			osmemset(semptr,0,sizeof(OSEVENT));
			ret = i;
			break;
		}
	}
	return ret;
}
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
*                                           QUEUEINIT
*
* Description: This function will init queue event
*
* Arguments  : void
*
* Returns    : fail---null,success---queue event pointer
*********************************************************************************************************
*/
OSEVENT * QqueueInit(U8 lenth,void *ptr)
{
	OSEVENT *queuemptr = NULL;
	INT16 i=0;
	for(i=0;i<EVENT_MAX_NUM;i++)
	{
		if(eventlist[i].eventType == INIT_EVENT)
		{
			eventlist[i].eventType = QUEUE_EVENT;
			eventlist[i].msgLenth = lenth;
			eventlist[i].msg = ptr;			
			queuemptr = &eventlist[i];
			break;
		}
	}
	return queuemptr;
}

/*
*********************************************************************************************************
*                                           QUEUEDEL
*
* Description: This function will delete queue event
*
* Arguments  : OSEVENT of queue will be delete
*
* Returns    : fail----1,success---queue event pointer
*********************************************************************************************************
*/
INT8 QueueDel(OSEVENT *queueptr)
{
	INT8 ret = -1;
	INT16 i=0;
	for(i=0;i<EVENT_MAX_NUM;i++)
	{
		if(queueptr == &eventlist[i])
		{
			osmemset(queueptr,0,sizeof(OSEVENT));
			ret = i;
			break;
		}
	}
	return ret;
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
