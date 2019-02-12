//
// lexical.h
//

#pragma once

#ifndef _LEXER_H
#define _LEXER_H
//防止重复包含该头文件

#include<stdio.h>
#include<stdlib.h>
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
	INT_CONST, UNSIGNED_CONST, LONG_CONST, UNSIGNED_LONG_CONST, DOUBLE_CONST, FLOAT_CONST,
	LONG_DOUBLE_CONST, CHAR_CONST, STRING_CONST, ASSIGN, PLUS, PLUSPLUS, MINUS, MINUSMINUS,
	MULTIPLY, DIVIDE, MOD, COMPARISON, AND, OR, LP, RP, LBT, RBT, LBS, RBS, SEMI, COMMA
};
//定义枚举类型，使不同类型的单词得到唯一的标识码

extern int num;						//行数计数器，记录当前已分析到源代码的第几行
extern char token_text[MAXLEN];		//存放单词值的字符串
extern keyword HashKey[25][11];		//存放关键字及其标识码的Hash表

int gettoken(FILE * fp);

#endif // !_LEXER_H