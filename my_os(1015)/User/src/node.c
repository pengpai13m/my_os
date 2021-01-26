#include <stdlib.h>
#include "timer.h"

#if SINGLY_LINKED_LIST_EN

typedef struct
{
	u8 id;			//Create the ID of the timer(the only)
	u8 idFlag;	//Create timer ID flag(id effectivity 1 | Invalid 0)
}PREVENT_REPEAT;

typedef struct
{
	u8 count;		//There are number of
	PREVENT_REPEAT node[MAX_TIMER];
}BACKUPS;

BACKUPS backups_node;

/********************************************************************
 * @name 		TIMER_ENUM add_backups_node(void)
 * @author 	surge
 * @date 		2021.1.25  
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM add_backups_node(u8 id)
{
	u8 i;
	for(i = 0; i < backups_node.count; i ++)
	{
		if(backups_node.node[i].id == id)
		{
			return FAIL;
		}
	}
	backups_node.node[backups_node.count].id = id;
	backups_node.node[backups_node.count].idFlag = SUCCEED;
	backups_node.count ++;
	return SUCCEED;
}

/********************************************************************
 * @name 		TIMER_ENUM delete_backups_node(u8 id)
 * @author 	surge
 * @date 		2021.1.25  
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM delete_backups_node(u8 id)
{
	u8 i;
	for(i = 0; i < backups_node.count; i ++)
	{
		if(backups_node.node[i].id == id)
		{
			backups_node.node[backups_node.count].id = 0xFF;
			backups_node.node[backups_node.count].idFlag = FAIL;
			for(;i < backups_node.count; i ++)		/*timer stask Swallow the forward*/
			{
				backups_node.node[backups_node.count - 1].id = backups_node.node[backups_node.count].id;
				backups_node.node[backups_node.count - 1].idFlag = backups_node.node[backups_node.count].idFlag;
			}
			backups_node.count --;
			return SUCCEED;
		}
	}
	return FAIL;
}

/********************************************************************
 * @name 		TIMER_ENUM delete_backups_list(void)
 * @author 	surge
 * @date 		2021.1.25  
 * @access  
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM delete_backups_list(void)
{
	u8 i;
	for(i = 0; i < MAX_TIMER; i ++)
	{
		backups_node.node[backups_node.count].id = 0xFF;
		backups_node.node[backups_node.count].idFlag = FAIL;
	}
	backups_node.count = 0;
	return SUCCEED;
}

/********************************************************************
 * @name 		LINK_NODE* create_node(LINK_NODE new_node)  
 * @author 	surge
 * @date 		2021.1.22  
 * @access  node creat
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
LINK_NODE* create_node(LINK_NODE new_node)
{
	LINK_NODE *pLinkNode = NULL;  
	if(new_node.id >= MAX_TIMER)
	{
		return pLinkNode; 
	}
	else
	{
		if(add_backups_node(new_node.id) == FAIL)
		{
			return pLinkNode;
		}
		else
		{
			pLinkNode = (LINK_NODE *)malloc(sizeof(LINK_NODE));        
			pLinkNode->id = new_node.id;
			pLinkNode->priority = new_node.priority;
			pLinkNode->p = new_node.p;
			pLinkNode->callBack = new_node.callBack;
			pLinkNode->next = NULL;  
		}
	}
	return pLinkNode;  
}

/********************************************************************
 * @name 		TIMER_ENUM _add_node(LINK_NODE** pNode, LINK_NODE* pDataNode)  
 * @author 	surge
 * @date 		2021.1.22  
 * @access  add node 
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM _add_node(LINK_NODE** pNode, LINK_NODE* pDataNode)
{
	if(NULL == *pNode) 
	{  
		*pNode = pDataNode;  
		return SUCCEED;  
	}
	return _add_node(&(*pNode)->next, pDataNode);  
}

/********************************************************************
 * @name 		TIMER_ENUM _add_node(LINK_NODE** pNode, LINK_NODE* pDataNode)  
 * @author 	surge
 * @date 		2021.1.22  
 * @access  add node 
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM add_node(const LINK_NODE** pNode, LINK_NODE new_node)  
{
	LINK_NODE *pDataNode;  
	if(NULL == *pNode) 
	{ 
		return FAIL;  
	}
	pDataNode = create_node(new_node);  
	if(pDataNode == NULL) 
	{
		return FAIL;
	}  
	return _add_node((LINK_NODE**)pNode, pDataNode);  
}

/********************************************************************
 * @name 		TIMER_ENUM _delete_node(LINK_NODE** pNode, u8 id)  
 * @author 	surge
 * @date 		2021.1.22  
 * @access  add node 
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM _delete_node(LINK_NODE** pNode, u8 id)  
{  
	LINK_NODE* pLinkNode; 
	if(NULL == (*pNode)->next) 
	{
		return FAIL;  
	}
	pLinkNode = (*pNode)->next;  
	if(id == pLinkNode->id) 
	{  
		(*pNode)->next = pLinkNode->next;  
		free(pLinkNode);  
		return SUCCEED;  
	} 
	else 
	{  
		return _delete_node(&(*pNode)->next, id);  
	}  
}  

/********************************************************************
 * @name 		TIMER_ENUM delete_node(LINK_NODE** pNode, u8 id)  
 * @author 	surge
 * @date 		2021.1.22  
 * @access  delete node 
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
TIMER_ENUM delete_node(LINK_NODE** pNode, u8 id)  
{
	LINK_NODE* pLinkNode;  
	if(NULL == pNode || NULL == *pNode) 
	{ 
		return FAIL;  
	}
	if(id == (*pNode)->id)
	{
		pLinkNode = *pNode;  
		*pNode = pLinkNode->next;  
		free(pLinkNode);  
		return SUCCEED;  
	}
	delete_backups_node(id);
	return _delete_node(pNode, id);  
}

/********************************************************************
 * @name 		LINK_NODE* find_node(const LINK_NODE* pLinkNode, u8 id)
 * @author 	surge
 * @date 		2021.1.22  
 * @access  find node 
 * @param		
 * @return  查找结点，返回数据内容为 id 的结点地址，没有找到返回NULL 
 * @version V1.0
 * @todo    null
********************************************************************/
LINK_NODE* find_node(const LINK_NODE* pLinkNode, u8 id) 
{  
	if(NULL == pLinkNode)
	{
		return NULL;  
	}
	if(id == pLinkNode->id)  
	{
		return (LINK_NODE*)pLinkNode;  
	}
	return find_node(pLinkNode->next, id);  
}

/********************************************************************
 * @name 		int count_list(const LINK_NODE *pLinkNode)  
 * @author 	surge
 * @date 		2021.1.22  
 * @access  count node 
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
int count_list(const LINK_NODE *pLinkNode)  
{  
	if(NULL == pLinkNode) 
	{  
		return 0;  
	}

	return 1 + count_list(pLinkNode->next);  
}

/********************************************************************
 * @name 		void print_list(const LINK_NODE *pLinkNode) 
 * @author 	surge
 * @date 		2021.1.22  
 * @access  print node 
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
void print_list(const LINK_NODE *pLinkNode)  
{  
	if(pLinkNode) 
	{  
		printf("%d\n", pLinkNode->id);  
		print_list(pLinkNode->next);  
	} 
}

/********************************************************************
 * @name 		void delete_list(LINK_NODE** pNode)  
 * @author 	surge
 * @date 		2021.1.22  
 * @access  delete node 
 * @param		
 * @return  
 * @version V1.0
 * @todo    null
********************************************************************/
void delete_list(LINK_NODE** pNode)  
{  
	LINK_NODE** pNext;
	if(NULL == pNode || NULL == *pNode) 
	{
		delete_backups_list();
		return ;  
	}    
	pNext = &(*pNode)->next;  
	free(*pNode);  
	delete_list(pNext);   
}
#endif
