#include"lexer.h"
#include "parser.h"
#include "tree.h"

int w;

/***********************************************************
*函数名称：program
*函数功能：分析当前语法成分是否为程序
*传入参数：源文件指针fp
*返回值：返回语法树AST。若不存在语法树，返回INFEASIBLE
***********************************************************/
CTreePtr program(FILE * fp)
{
	CTreePtr T;
	CTreePtr c;
	char string[] = "程序";
	InitTree(T);
	w = gettoken(fp);
	if ((c = ExtDefList(fp)) && c->n)
	{
		T->n = 1; T->r = 0;
		T->nodes[0].data = string;		//定义一个结点作为空树的根
		T->nodes[0].firstchild = NULL;
		if (!InsertChild(T, T->r, 1, c)) return NULL;
	}
	else return NULL;
	return T;
}

/***********************************************************
*函数名称：program
*函数功能：分析当前语法成分是否为程序
*传入参数：源文件指针fp
*返回值：返回语法树AST。若不存在语法树，返回INFEASIBLE
***********************************************************/
CTreePtr ExtDefList(FILE * fp)
{
	CTreePtr T; CTreePtr c;
	char string[] = "外部定义序列";
	if (w == EOF) return NULL;
	InitTree(T);
	T->n = 1;  T->r = 0;
	T->nodes[0].data = string;
	T->nodes[0].firstchild = NULL;
	c = ExtDef(fp);
	InsertChild(T, T->r, 1, c);
	c = ExtDefList(fp);
	InsertChild(T, T->r, 2, c);
	return T;
}
