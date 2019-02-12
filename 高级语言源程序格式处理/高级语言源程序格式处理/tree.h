//
// tree.h
//
// Description
// 按照孩子表示法构建了一棵树。

#pragma once

#ifndef _TREE_H
#define _TREE_H

#include<stdio.h>

#define MAX_TREE_SIZE 100
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;
typedef int TElemType;
// - - - - - 树的孩子链表存储表示 - - - - -
typedef struct CTNode		//孩子结点
{
	int child;
	CTNode * next;
}*ChildPtr;
typedef struct
{
	TElemType data;
	ChildPtr firstchild;	//孩子链表头结点
}CTBox;
typedef struct
{
	CTBox nodes[MAX_TREE_SIZE];
	int n, r;				//结点数和根的位置
}CTree;

Status InitTree(CTree & T);
Status CreateTree(CTree & T, TElemType);

#endif // !_TREE_H

