#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f10x.h"
#include <math.h>
#include <stdio.h>

#define CLOCKS_PER_SEC 					1000
#define	MAX_TIMER								32 	/*(1 ~ 31)*/

#define CIRCULAR_LINKED_LIST_EN	1		/*circular linked list en*/
#define SINGLY_LINKED_LIST_EN		0		/*Singly linked list en*/

typedef void (*timer_handler) (void *p);

typedef enum
{
	SUCCEED,
	FAIL,
	RETRY
}TIMER_ENUM;

typedef enum
{
	APP_TIMER_MODE_ONCE,     	  /*The timer will expire only once. */
	APP_TIMER_MODE_REPEATED,    /*The timer will restart each time it expires. */
	APP_TIMER_MODE_MAX
}TIMER_MODE;

typedef enum
{
	TIMER_INIT,            			/*The timer node has been allocated. */
	TIMER_RUNNING,         			/*The timer is running. */
	TIMER_STOP,            			/*The timer is be stop, but not delete form list. */
	TIMER_DELETE,          			/*The timer node has been delete from list. or */
	TIMER_ERR,
}TIMER_RUN_STATE;

typedef struct _LINK_NODE  
{  
	/*User Settings*/
	u8 id;											/*Create the ID of the timer(the only)*/  
	/*User Settings*/		
//	u8 priority;								/*priority*/
	TIMER_RUN_STATE Flag;				/*Timer state*/
	/*User Settings*/		
	TIMER_MODE mode;						/*Timer Run Mode*/
	/*User Settings*/		
	u32 time;										/*Timer Delayed execution time*/
	u32	startTime;							/*Record the timer start time*/
	/*User Settings*/		
	timer_handler	callBack;			/*callback*/
	/*User Settings*/		
	void *p; 										/*Used to pass the callback parameter*/
	struct _LINK_NODE* next;  
}LINK_NODE; 

extern TIMER_ENUM system_timer_init(void);

extern TIMER_ENUM system_timer_creat(u8 id,u8 priority,timer_handler p_handle);

extern TIMER_ENUM system_timer_start(u8 id,TIMER_MODE mode,u32 time,void *p);

extern TIMER_ENUM system_timer_stop(u8 id);

extern TIMER_ENUM system_timer_delete(u8 id);

extern TIMER_RUN_STATE system_get_timer_state(u8 id);

extern TIMER_ENUM system_timer_run(void);

extern u32 g_Systemtime;						/*系统运行时间 单位 1ms*/
#endif
