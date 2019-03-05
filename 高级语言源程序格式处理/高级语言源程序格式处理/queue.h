//
// queue.h
//
// Description
// 构建了一个链队列。

#pragma once

#ifndef _QUEUE_H
#define _QUEUE_H

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<stack>

#define OK 1
#define ERROR 0
#define INFEASIBLE -1

typedef int QElemType;	//定义元素类型
typedef int Status;
// - - - - - 单链队列——队列的链式存储结构 - - - - -
typedef struct QNode
{
	QElemType data;
	QNode * next;
}QNode, *QueuePtr;
typedef struct
{
	QueuePtr front;		//队头指针
	QueuePtr rear;		//队尾指针
}LinkQueue;

Status InitQueue(LinkQueue & Q);
Status DestroyQueue(LinkQueue & Q);
Status ClearQueue(LinkQueue & Q);
int QueueLength(LinkQueue Q);
Status EnQueue(LinkQueue & Q, QElemType e);
Status DeQueue(LinkQueue & Q, QElemType & e);
Status QueueTraverse(LinkQueue Q, Status(*visit)(QElemType));

#endif // !_QUEUE_H
