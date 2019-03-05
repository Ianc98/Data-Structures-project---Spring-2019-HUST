//
// stack.h
//
// Description
// 构建了一个顺序栈。

#pragma once

#ifndef _STACK_H
#define _STACK_H

#include<stdlib.h>
#include<malloc.h>
#include"tree.h"

#define OK 1
#define ERROR 0
typedef int Status;
typedef CTree * SElemType;
// - - - - - 栈的顺序存储表示 - - - - -
#define STACK_INIT_SIZE 100		//存储空间初始分配量
#define STACKINCREMENT 10		//存储空间分配增量
typedef struct
{
	SElemType * base;			//在栈构造之前和销毁之后，base的值为NULL
	SElemType * top;			//栈顶指针
	int stacksize;				//当前已分配的存储空间，以元素为单位
}SqStack;

Status InitStack(SqStack &S);
Status GetTop(SqStack S, SElemType &e);
Status Push(SqStack &S, SElemType e);
Status Pop(SqStack &S, SElemType &e);
#endif // !_STACK_H
