#include "tree.h"
#include "stack.h"
using namespace std;

/**********************************************
*函数名称：InitTree
*函数功能：对树进行初始化
*传入参数：树CTree T
*返回值：函数执行状态。执行成功返回OK。
**********************************************/
Status InitTree(CTree & T)
{
	T.n = 0; T.r = -1;		//将根的位置设为-1，说明当前树没有根
	return OK;
}

/**********************************************
*函数名称：InsertChild
*函数功能：为T中某个结点插入一棵子树
*传入参数：树CTree T，int p指向某个结点，int i
表示第i棵子树，CTree c是要插入的子树
*返回值：函数执行状态。执行成功返回OK。否则返回
ERROR
**********************************************/
Status InsertChild(CTree & T, int p, int i, CTree c)
{
	if (p < 0 || p >= T.n) return ERROR;			//p不指向T的某个结点
	if (!c.n) return ERROR;							//要插入的子树是空树
	int k = 0, j = T.n;
	ChildPtr prior = T.nodes[p].firstchild;			//用来查找第i个结点的前一个结点
	ChildPtr cur = prior;							//用来查找第i个结点
	ChildPtr t;
	while (k < c.n)
	{
		T.nodes[j] = c.nodes[k++];
		for (t = T.nodes[j].firstchild; t; t = t->next)
			t->child += T.n;
		j++;
	}
	t = (ChildPtr)malloc(sizeof(CTNode));
	if (!t) exit(OVERFLOW);
	t->child = T.n + c.r;							//子树根的位置
	t->next = NULL;
	T.n += c.n;
	if (i == 1)
	{
		if (prior)
		{
			t->next = prior;
			prior = t;
		}
		else prior = t;
		T.nodes[p].firstchild = prior;
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

Status GetParent(CTree T, int child, int & parent)
{
	ChildPtr t; int i = 0;
	if (child >= T.n) return ERROR;
	for (; i < T.n; i++)
	{
		t = T.nodes[i].firstchild;
		while (t&&t->child != child)
			t = t->next;
		if (t&&t->child == child)
		{
			parent = i;
			return OK;
		}
	}
	return INFEASIBLE;
}

/****************************************************
*函数名称：TraverseTree
*函数功能：对树T进行先根遍历
*传入参数：树CTree T，对结点操作的应用函数visit()
*返回值：函数执行状态。
*****************************************************/
Status TraverseTree(CTree T, Status(*visit)(TElemType, int))
{
	stack<int> stack;
	int status = 0;							//用来标志是否所有孩子都被访问过
	int i; int j;
	int indent = 0;							//缩进量
	int parent;
	ChildPtr t;
	int visited[MAX_TREE_SIZE] = { 0 };		//访问标志数组初始化
	if (!T.n) return OK;					//树为空直接结束
	for (i = 0; i < T.n; i++)
	{
		indent = 0;
		if (!visited[i])					//对尚未访问的结点进行访问
		{
			stack.push(i);
			visited[i] = 1;
			if (GetParent(T, i, parent) == OK)
				indent += T.nodes[parent].indent;
			if (T.nodes[i].indent)
				visit(T.nodes[i].data, indent);
			t = T.nodes[i].firstchild;
			while (t && !status)									//如果该结点有孩子
			{
				while (visited[t->child] && t->next)	//寻找未被访问过的孩子
					t = t->next;
				if (!visited[t->child])
				{
					visited[t->child] = 1;
					if(T.nodes[t->child].firstchild)
						stack.push(t->child);
					GetParent(T, t->child, parent);
					indent += T.nodes[parent].indent;
					if (T.nodes[t->child].indent)
						visit(T.nodes[t->child].data, indent);
					t = T.nodes[t->child].firstchild;
					status = 0;
				}
				else status = 1;
			}
		}
		while (!stack.empty())						//循环到栈为空
		{
			j = stack.top();
			indent -= T.nodes[j].indent;
			t = T.nodes[j].firstchild;
			status = 0;
			while (t && !status)							//如果该结点有孩子
			{
				while (visited[t->child] && t->next)	//寻找未被访问过的孩子
					t = t->next;
				if (!visited[t->child])
				{
					visited[t->child] = 1;
					if(T.nodes[t->child].firstchild)
						stack.push(t->child);
					GetParent(T, t->child, parent);
					indent += T.nodes[parent].indent;
					if (T.nodes[t->child].indent)
						visit(T.nodes[t->child].data, indent);
					t = T.nodes[t->child].firstchild;
					status = 0;
				}
				else status = 1;
			}
			if (t) stack.pop();
		}
	}
	return OK;
}