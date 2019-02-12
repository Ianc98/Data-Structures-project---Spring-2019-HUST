#include "tree.h"

/**********************************************
*函数名称：InitTree
*函数功能：对树进行初始化
*传入参数：树CTreePtr T
*返回值：函数执行状态。执行成功返回OK。
**********************************************/
Status InitTree(CTreePtr & T)
{
	if (!T) T = (CTreePtr)malloc(sizeof(CTree));
	if (!T) exit(OVERFLOW);
	T->n = 0; T->r = -1;		//将根的位置设为-1，说明当前树没有根
	return OK;
}

/**********************************************
*函数名称：InsertChild
*函数功能：为T中某个结点插入一棵子树
*传入参数：树CTreePtr T，int p指向某个结点，int i
表示第i棵子树，CTreePtr c是要插入的子树
*返回值：函数执行状态。执行成功返回OK。否则返回
ERROR
**********************************************/
Status InsertChild(CTreePtr & T, int p, int i, CTreePtr c)
{
	if (!T) return ERROR;
	if (p < 0 || p >= T->n) return ERROR;			//p不指向T的某个结点
	if (!c || !c->n) return ERROR;					//要插入的子树是空树
	int k = 0, j = T->n;
	ChildPtr prior = T->nodes[p].firstchild;			//用来查找第i个结点的前一个结点
	ChildPtr cur = prior;							//用来查找第i个结点
	ChildPtr t = (ChildPtr)malloc(sizeof(CTNode));
	if (!t) exit(OVERFLOW);
	while (k < c->n)
		T->nodes[j++] = c->nodes[k++];
	t->child = T->n + c->r;							//子树根的位置
	T->n += c->n;
	if (i == 1)
	{
		if (prior)
		{
			t->next = prior;
			prior = t;
		}
		else prior = t;
	}
	else if (i < 1) return ERROR;					//如果i输入非法，返回ERROR
	else
	{
		k = 1;											//k表示当前是第几棵子树
		while (k < i&&cur)
		{
			k++;
			prior = cur;
			cur = cur->next;
		}
		if (cur)
		{
			prior->next = t;
			t->next = cur;
		}
		else if (k == i)
		{
			prior->next = t;
			t->next = NULL;
		}
		else return ERROR;
	}
	return OK;
}
