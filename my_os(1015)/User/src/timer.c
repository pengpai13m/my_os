#include "timer.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"

#if SINGLY_LINKED_LIST_EN
extern LINK_NODE* create_node(LINK_NODE new_node);
extern TIMER_ENUM add_node(const LINK_NODE** pNode, LINK_NODE new_node);
extern LINK_NODE* find_node(const LINK_NODE* pLinkNode, u8 id);
extern TIMER_ENUM delete_node(LINK_NODE** pNode, u8 id);
LINK_NODE *TimerLink = NULL;
#endif

#if CIRCULAR_LINKED_LIST_EN
extern LINK_NODE *creat_circular_node(LINK_NODE Param);
extern TIMER_ENUM insert_circular_node(LINK_NODE *head,u8 id,TIMER_RUN_STATE state,timer_handler	callBack);
extern TIMER_ENUM update_circular_list(LINK_NODE *head,u8 id,TIMER_MODE mode,u32 time,TIMER_RUN_STATE state,void *p);
extern TIMER_ENUM delete_circular_list(LINK_NODE *head,u8 id);
extern TIMER_RUN_STATE seek_circular_list(LINK_NODE*head,u8 id);
extern TIMER_ENUM display_circular_list(LINK_NODE *head);
LINK_NODE *TimerLinkC = NULL;
#endif

u32 g_Systemtime;

/********************************************************************
 * @name 		void system_timer_init(void)
 * @author 	surge
 * @date 		2021.1.20  
 * @access  system_cofig
 * @param		void
 * @return  SUCCEED  FAIL  RETRY
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM system_timer_init(void)
{
	SystemInit();
  /* 重设中断向量表 */
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x4000);
  /* Setup SysTick Timer for xx ms interrupts */
  /* 100->10ms, 1000->1ms, 400->2.5ms, 10000->0.1ms */
  if (SysTick_Config(SystemCoreClock / CLOCKS_PER_SEC))
  {
    /* Capture error */ 
    return FAIL;
  }
	return SUCCEED;
}

/********************************************************************
 * @name 		u8 SysTick_Handler(void)
 * @author 	surge
 * @date 		2021.1.22  
 * @access  system timer handler
 * @param		void
 * @return  void
 * @version V1.0
 * @todo    null
********************************************************************/
void SysTick_Handler(void)  //1ms
{
	g_Systemtime += 1000/CLOCKS_PER_SEC;
}

/********************************************************************
 * @name 		u8 system_timer_creat(u8 id,u8 type,timer_callback callback)
 * @author 	surge
 * @date 		2020.10.15  
 * @access  system timer creat
 * @param		id-- timer id   | type-- timer type | p_handle-- timer callback
 * @return  Indicates whether the timer was created successfully  SUCCEED  FAIL  RETRY
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM system_timer_creat(u8 id,u8 priority,timer_handler p_handle)
{
	if(id >= MAX_TIMER)
	{
		return FAIL;
	}
#if SINGLY_LINKED_LIST_EN
	LINK_NODE link_node;
	if(TimerLink == NULL)
	{
		link_node.id = 0;
		link_node.priority = 0;
		link_node.callBack = NULL;
		TimerLink = create_node(link_node);
		if(TimerLink != NULL)
		{
			return SUCCEED;
		}
		return FAIL;
	}
	else
	{
	}
	link_node.id = id;
	link_node.priority = priority;
	link_node.callBack = p_handle;
	link_node.Flag = TIMER_INIT;
	if(add_node((const LINK_NODE**)&TimerLink,link_node) == TRUE)
	{
		return SUCCEED;
	}

	return FAIL;
#endif
	
#if CIRCULAR_LINKED_LIST_EN
	if(TimerLinkC == NULL)
	{
		LINK_NODE new_node;
		new_node.id = id - 1;
//		new_node.priority = priority;
		new_node.callBack = p_handle;
		TimerLinkC = creat_circular_node(new_node);
		if(TimerLinkC == NULL)
		{
			return FAIL;
		}
	}
	else
	{
	}
	return insert_circular_node(TimerLinkC,id,TIMER_INIT,p_handle);
#endif

}

/********************************************************************
 * @name 		u8 system_timer_creat(u8 id,u8 type,timer_callback callback)
 * @author 	surge
 * @date 		2020.10.15  
 * @access  system timer start
 * @param		id-- timer id  | mode-- work mode | time-- timer  triggering time ms
 * @return  SUCCEED  FAIL  RETRY
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM system_timer_start(u8 id,TIMER_MODE mode,u32 time,void *p)
{
	if(id >= MAX_TIMER)
	{
		return FAIL;
	}
#if SINGLY_LINKED_LIST_EN
	LINK_NODE *link_node;
	link_node = find_node((const LINK_NODE*)&TimerLink,id);
	link_node->mode = mode;
	link_node->time = time;
	link_node->Flag = TIMER_RUNNING;
	
	return SUCCEED;
#endif	

#if CIRCULAR_LINKED_LIST_EN
	
	return update_circular_list(TimerLinkC,id,mode,time,TIMER_RUNNING,p);
#endif
	
}

/********************************************************************
 * @name 		u8 system_timer_stop(u8 id)
 * @author 	surge
 * @date 		2020.10.15  
 * @access  system timer stop
 * @param		id-- timer id   
 * @return  Timer stop SUCCEED  FAIL  RETRY
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM system_timer_stop(u8 id)
{
	if(id >= MAX_TIMER)
	{
		return FAIL;
	}
#if SINGLY_LINKED_LIST_EN
	LINK_NODE *link_node;
	link_node = find_node((const LINK_NODE*)&TimerLink,id);
	if(link_node == NULL)
	{
		return FAIL;
	}
	link_node->Flag = TIMER_STOP;
	return SUCCEED;
#endif

#if CIRCULAR_LINKED_LIST_EN
	return update_circular_list(TimerLinkC,id,APP_TIMER_MODE_MAX,0,TIMER_STOP,NULL);
#endif
}

/********************************************************************
 * @name 		TIMER_ENUM system_timer_delete(u8 id)
 * @author 	surge
 * @date 		2020.10.15  
 * @access  system timer delete
 * @param		timer_node_t-- timer    
 * @return  delete timer SUCCEED  FAIL  RETRY
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM system_timer_delete(u8 id)
{
	if(id >= MAX_TIMER)
	{
		return FAIL;
	}
#if SINGLY_LINKED_LIST_EN
	if(delete_node((LINK_NODE**)&TimerLink,id) == TRUE)
	{
		return SUCCEED;
	}
	return FAIL;
#endif
	
#if CIRCULAR_LINKED_LIST_EN
	return delete_circular_list(TimerLinkC,id);
#endif

}

/********************************************************************
 * @name 		TIMER_ENUM system_get_timer_state(u8 id)
 * @author 	surge
 * @date 		2020.10.15  
 * @access  system timer delete
 * @param		timer_node_t-- timer    
 * @return  Run state of timer  SUCCEED  FAIL  RETRY
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_RUN_STATE system_get_timer_state(u8 id)
{
	if(id >= MAX_TIMER)
	{
		return TIMER_ERR;
	}
#if SINGLY_LINKED_LIST_EN
	LINK_NODE *link_node;
	link_node = find_node((const LINK_NODE*)&TimerLink,id);
	if(link_node == NULL)
	{
		return TIMER_ERR;
	}
	else
	{
	}	
	return link_node->Flag;
	
#endif

#if CIRCULAR_LINKED_LIST_EN
	return seek_circular_list(TimerLinkC,id);
#endif
}

/********************************************************************
 * @name 		TIMER_ENUM system_timer_run(void)
 * @author 	surge
 * @date 		2020.10.15  
 * @access  
 * @param		
 * @return  Run state of timer  SUCCEED  FAIL  RETRY
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM system_timer_run(void)
{
#if SINGLY_LINKED_LIST_EN
	for(; TimerLink != NULL; )
	{
		if(TimerLink->Flag == TIMER_RUNNING)
		{
			
		}
		TimerLink = TimerLink->next;
	}
#endif

#if CIRCULAR_LINKED_LIST_EN
	display_circular_list(TimerLinkC);
#endif
	return SUCCEED;
}
