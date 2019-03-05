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
#include<stack>

#define MAX_TREE_SIZE 1000
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

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
	int indent;				//该结点的孩子要增加的缩进量
	ChildPtr firstchild;	//孩子链表头结点
}CTBox;
typedef struct
{
	CTBox nodes[MAX_TREE_SIZE];
	int n, r;				//结点数和根的位置
}CTree;

Status InitTree(CTree & T);
Status InsertChild(CTree & T, int p, int i, CTree c);
Status GetParent(CTree T, int child, int & parent);
Status TraverseTree(CTree T, Status(*visit)(TElemType, int));

#endif // !_TREE_H

