
#ifndef   OS_TASK_H
#define   OS_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"

#define    TIMER_MAX_NUM     8
#define    TASK_NAME_LEN     16
#define    TASK_TICK         10                   /*systick time 10ms*/
#define    DP printf
#define    MAX_PRIO 16
#define    RTT_SCHDULE       0
#define    FAIR_SCHDULE      1

#define    INIT_EVENT        0
#define    SEM_EVENT         1
#define    MUTEX_EVENT       2
#define    QUEUE_EVENT       3

#define    SEM_PEND          1
#define    MUTEX_PEND        2
#define    QUEUE_PEND        3

#define    TASK_DEALY        1
#define    TASK_READY        2
#define    TASK_RUNNING      3
#define    TASK_CLOSE        4

#define    ERR_TYPE          1
#define    ERR_PRT           2
#define    ERR_NUM           3

typedef unsigned char* OSSTK;

typedef struct os_event {
	U8               eventType;
	U8               eventGrp;          /*===waiting event prio*/
	U8               eventRdy[4];        
	void             *msg;              /**/
} OSEVENT;

typedef struct os_time {
	U8               timeId;
	U8               prio;
	U16              tickLeft;        
} OSTIME;

typedef struct os_tcb {
    OSSTK            *pStack;           /* Pointer to current top of stack                         */
    //struct os_tcb    *pTCBNext;             /* Pointer to next     TCB in the TCB list                 */
    //struct os_tcb    *pTCBPrev;             /* Pointer to previous TCB in the TCB list                 */
    OSEVENT          *pEventptr;         /* Pointer to          event control block                 */
	//U8               name[TASK_NAME_LEN];
	U8               prio;
	U8               taskState;         
	U8               pendState;          /*real time or time slice*/
	//U8               timeout;           /*255*10ms*/
} OSTCB;

typedef struct os_control {
	U8               timerNum;
	U8               scheduleType;      /* time slice or rtt*/       
} OSCONTROL;

extern OSTCB taskTbl[MAX_PRIO];  //384byte
extern OSTCB *current;
extern OSCONTROL osGlobal;

U8  SemPend(OSEVENT *event,U32 timeout);
U8  SemPost(OSEVENT *event);
void TaskDelay(U32 delay);
U32 GetRdyHighPrio(U8 *group,U8 *RdyTbl);
void SetRdyPrio(U32 prio,U8 *group,U8 *RdyTbl);
void ClearRdyPrio(U32 prio,U8 *group,U8 *RdyTbl);
void OsSched(void);
#ifdef __cplusplus
}
#endif

#endif
