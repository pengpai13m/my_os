#include <stdlib.h>
#include <stdio.h>
#include "timer.h"

#if CIRCULAR_LINKED_LIST_EN

extern u32 g_Systemtime;
/********************************************************************
 * @name 		LINK_NODE *creat_circular_node(LINK_NODE Param)
 * @author 	surge
 * @date 		2021.1.22   
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    Create a node
********************************************************************/
LINK_NODE *creat_circular_node(u8 id,TIMER_RUN_STATE state,timer_handler callBack)/*创建节点*/
{
	LINK_NODE *newnode = (LINK_NODE *)malloc(sizeof(LINK_NODE));
	if(newnode == NULL)
	{
		return NULL;
	}
	else
	{
		newnode->id = id;
//		newnode->priority = priority;
		newnode->Flag = state;
		newnode->callBack = callBack;
		newnode->next = newnode; 
	}
	return newnode;
	
//	if(newnode == NULL)
//	{
//		return NULL;
//	}
//	else
//	{
//		newnode->id = id;
////		newnode->priority = priority;
//		newnode->Flag = state;
//		newnode->callBack = callBack;
//		newnode->next = newnode; 
//	}
//	return newnode;
}

/********************************************************************
 * @name 		TIMER_ENUM insert_circular_node(LINK_NODE *head,LINK_NODE Param)
 * @author 	surge
 * @date 		2021.1.22   
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    Add a new node
********************************************************************/
TIMER_ENUM insert_circular_node(LINK_NODE *head,u8 id,TIMER_RUN_STATE state,timer_handler	callBack)/*添加新节点*/
{
	LINK_NODE *newnode = creat_circular_node(id,state,callBack);
	if(head != NULL)
	{
		LINK_NODE *p  = head;
		while(p->next != head)
		{
			p = p->next;
		}
		p->next = newnode;
		newnode->next = head;
		return SUCCEED;
	}
	else
	{
		return FAIL;
	}
//	if(head != NULL)
//	{
//		LINK_NODE *p  = head;
//		while(p->next != head)
//		{
//			p = p->next;
//		}
//		p->next = newnode;
//		newnode->next = head;
//		return SUCCEED;
//	}
//	else
//	{
//		return FAIL;
//	}
}

/********************************************************************
 * @name 		TIMER_ENUM update_circular_list(LINK_NODE *head,u8 id,TIMER_MODE mode,u32 time,TIMER_RUN_STATE state)
 * @author 	surge
 * @date 		2021.1.22   
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    Update node data
********************************************************************/
TIMER_ENUM update_circular_list(LINK_NODE *head,u8 id,TIMER_MODE mode,u32 time,TIMER_RUN_STATE state,void *p)/*更新节点数据*/
{
	if(head == NULL)
	{
		return FAIL;
	}
	LINK_NODE *temp = head;
	do
	{
		temp = temp->next;
		if(temp->id == id)
		{
//			temp->next->id = id;/*change*/
			temp->mode = mode;
			temp->time = time;
			temp->Flag = state;
			temp->p = p;
			if(state == TIMER_RUNNING)
			{
				temp->startTime = g_Systemtime;
			}
			return SUCCEED;
		}
		
	}while(temp->next != head);
	return FAIL;
}

/********************************************************************
 * @name 		TIMER_ENUM delete_circular_list(LINK_NODE *head,u8 id)
 * @author 	surge
 * @date 		2021.1.22   
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    Remove nodes
********************************************************************/
TIMER_ENUM delete_circular_list(LINK_NODE *head,u8 id)/*删除节点*/
{
	if(head == NULL)
	{
		return FAIL;
	}
	LINK_NODE *temp = head;
	LINK_NODE *ptr = head->next;
	do
	{
		if(ptr->id == id)
		{
			if(ptr->next == head)//循环结束的条件换成ptr->next == head
			{
				temp->next = head;
				free(ptr);
				return SUCCEED;
			}
			temp->next = ptr->next;
			free(ptr);
			return SUCCEED;
		}
		temp = temp->next;
		ptr = ptr->next;
	}while(ptr != head);
	return FAIL;
}

/********************************************************************
 * @name 		TIMER_RUN_STATE seek_circular_list(LINK_NODE*head,u8 id)
 * @author 	surge
 * @date 		2021.1.22   
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    Query the timer status of the linked list corresponding to the id
********************************************************************/
TIMER_RUN_STATE seek_circular_list(LINK_NODE*head,u8 id)/*查询链表对应id的timer状态*/
{
	if(head == NULL)
	{
		return TIMER_ERR;
	}
	LINK_NODE *p = head;
	do
	{
		p = p->next;
		if(p->id == id)
		{
			return p->Flag;
		}
		
	}while(p->next != head);
	return TIMER_ERR;
}

/********************************************************************
 * @name 		TIMER_ENUM display_circular_list(LINK_NODE *head)
 * @author 	surge
 * @date 		2021.1.22   
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    null Iterate through the list data
********************************************************************/
TIMER_ENUM display_circular_list(LINK_NODE *head)/**/
{
	static LINK_NODE *p;
	if(head != NULL)
	{
		if(p == NULL)
		{
			p = head;
		}
		do
		{
			if(p->Flag == TIMER_RUNNING)
			{
				if((abs((int)(g_Systemtime - p->startTime))*1000/CLOCKS_PER_SEC)%p->time == 0)
				{
					p = p->next;
					if(p->mode == APP_TIMER_MODE_ONCE)
					{
						p->Flag = TIMER_STOP;
					}
					else
					{
					}
					p->callBack(p->p);
					return SUCCEED;
				}
				else
				{
					p = p->next;
				}
			}
			else
			{
				p = p->next;
				return RETRY;
			}
		}while(p->next != head );	
	}
	return FAIL;
}

#endif
