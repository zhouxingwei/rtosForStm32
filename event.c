#include "easyos.h"

U8  SemPend(OSEVENT *event,U32 timeout)
{
	if(event == NULL || event->eventType != SEM_EVENT)
	{
		return ERR_TYPE;
	}
	SetRdyPrio(current->prio,&event->eventGrp,&event->eventRdy[0]);
	ClearRdyPrio(current->prio,&RdyGroup,&RdyTbl[0]);
	OsSched();
}

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
}
