#include"lexer.h"
#include "parser.h"
#include "tree.h"

token_kind w;

/**********************************************
*函数名称：program
*函数功能：分析当前语法成分是否为程序
*传入参数：源文件指针fp
*返回值：？？？
**********************************************/
Status program(FILE * fp)
{
	CTree T;
	InitTree(T);
	w = gettoken(fp);
	if (ExtDefList(fp))
	{
		
	}
	return Status();
}
