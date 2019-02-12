//
// tree.h
//
// Description
// 按照孩子表示法构建了一棵树。

#pragma once

#ifndef _TREE_H
#define _TREE_H

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define MAX_TREE_SIZE 100
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;
typedef char * TElemType;
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
}CTree, *CTreePtr;

Status InitTree(CTreePtr & T);
Status InsertChild(CTreePtr & T, int p, int i, CTreePtr c);

#endif // !_TREE_H

