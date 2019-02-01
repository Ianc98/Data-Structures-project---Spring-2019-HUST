//
// lexical.h
//

#pragma once

#ifndef _LEXICAL_H
#define _LEXICAL_H
//防止重复包含该头文件

#include<stdio.h>
#include<malloc.h>
#include<string.h>

#define MAXLEN 100

struct keyword
{
	char key[15];
	enum token_kind c;
};
//Hash表的数据元素
enum token_kind
{
	ERROR_TOKEN, AUTO, BREAK, CASE, CHAR, CONST, CONTINUE, DEFAULT, DO, DOUBLE, ELSE, ENUM, EXTERN,
	FLOAT, FOR, GOTO, IF, INLINE, INT, LONG, REGISTER, RESTRICT, RETURN, SHORT, SIGNED, SIZEOF,
	STATIC, STRUCT, SWITCH, TYPEDEF, UNION, UNSIGNED, VOID, VOLATILE, WHILE, _ALIGNAS, _ALIGNOF,
	_ATOMIC, _BOOL, _COMPLEX, _GENERIC, _IMAGINARY, _NORETURN, _STATIC_ASSERT, _THREAD_LOCAL, IDENT,
	INT_CONST, FLOAT_CONST, EQ, ASSIGN, PLUS, PLUSPLUS, SEMI
};
//定义枚举类型，使不同类型的单词得到唯一的标识码

extern int num;						//行数计数器，记录当前已分析到源代码的第几行
extern char token_text[MAXLEN];		//存放单词值的字符串
extern keyword HashKey[25][11];		//存放关键字及其标识码的Hash表

/**********************************************
*函数名称：gettoken
*函数功能：对源代码进行词法分析
*传入参数：源代码文件指针fp
*返回值：单词标识码
**********************************************/
int gettoken(FILE * fp);

#endif // !_LEXICAL_H