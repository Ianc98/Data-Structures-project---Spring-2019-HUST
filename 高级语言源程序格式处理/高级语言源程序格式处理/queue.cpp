#include "queue.h"

/**********************************************
*函数名称：InitQueue
*函数功能：构造一个空队列Q
*传入参数：LinkQueue Q
*返回值：函数执行状态，如果创建成功，返回OK
**********************************************/
Status InitQueue(LinkQueue & Q)
{
	Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q.front) exit(OVERFLOW);						//存储分配失败
	Q.front->next = NULL;
	return OK;
}

/**********************************************
*函数名称：DestroyQueue
*函数功能：销毁队列Q,Q不再存在
*传入参数：LinkQueue Q
*返回值：函数执行状态，如果销毁成功，返回OK
**********************************************/
Status DestroyQueue(LinkQueue & Q)
{
	while (Q.front)
	{
		Q.rear = Q.front->next;
		free(Q.front);
		Q.front = Q.rear;
	}
	return OK;
}

/**********************************************
*函数名称：ClearQueue
*函数功能：Q存在时，清空队列Q
*传入参数：LinkQueue Q
*返回值：函数执行状态，如果清空成功，返回OK；
否则返回ERROR
**********************************************/
Status ClearQueue(LinkQueue & Q)
{
	if (!Q.front) return ERROR;
	while (Q.front->next)
	{
		Q.rear = Q.front->next;
		Q.front->next = Q.front->next->next;
		free(Q.rear);
	}
	Q.rear = Q.front;
	return OK;
}

/***********************************************************
*函数名称：QueueLength
*函数功能：如果队列存在，返回队列元素个数
*传入参数：LinkQueue Q
*返回值：队列元素个数，如果队列不存在，返回INFEASIBLE
***********************************************************/
int QueueLength(LinkQueue Q)
{
	QueuePtr t; int i = 0;
	if (!Q.front) return INFEASIBLE;
	t = Q.front->next;
	while (t)
	{
		t = t->next;
		i++;
	}
	return i;
}

/**********************************************************************
*函数名称：EnQueue
*函数功能：如果队列存在，插入元素e为Q的新的队尾元素
*传入参数：队列LinkQueue Q，要插入的元素QElemType e
*返回值：函数执行状态，如果插入成功，返回OK；如果队列不存在，返回ERROR
***********************************************************************/
Status EnQueue(LinkQueue & Q, QElemType e)
{
	QueuePtr p;
	if (!Q.front) return ERROR;
	p = (QueuePtr)malloc(sizeof(QNode));
	if (!p) exit(OVERFLOW);
	p->data = e; p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}

/**********************************************************************
*函数名称：DeQueue
*函数功能：若队列不空，则删除Q的队头元素，用e返回其值
*传入参数：队列LinkQueue Q，删除的元素QElemType e
*返回值：函数执行状态，如果删除成功，返回OK；如果队列为空或不存在，返回ERROR
***********************************************************************/
Status DeQueue(LinkQueue & Q, QElemType & e)
{
	QueuePtr p;
	if (Q.front == Q.rear) return ERROR;
	p = Q.front->next;
	e = p->data;
	Q.front->next = p->next;
	if (Q.rear == p) Q.rear = Q.front;
	free(p);
	return OK;
}

/**********************************************************************
*函数名称：QueueTraverse
*函数功能：若队列已存在且非空，则从队头到队尾，依次对Q的每个数据元素
调用函数visit()。一旦visit()失败，则操作失败
*传入参数：队列LinkQueue Q，函数visit()
*返回值：函数执行状态，如果遍历成功，返回OK；否则返回ERROR
***********************************************************************/
Status QueueTraverse(LinkQueue Q, Status(*visit)(QElemType))
{
	QueuePtr p;
	if (Q.front == Q.rear) return ERROR;
	p = Q.front->next;
	while (p)
	{
		visit(p->data);
		p = p->next;
	}
	return OK;
}
