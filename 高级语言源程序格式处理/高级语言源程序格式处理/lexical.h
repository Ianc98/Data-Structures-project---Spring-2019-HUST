//
// lexical.h
//

#pragma once

#ifndef _LEXICAL_H
#define _LEXICAL_H

#include<stdio.h>
#include<malloc.h>
#include<string.h>

#define MAXLEN 100

struct keyword
{
	char key[10];
	enum token_kind c;
};
enum token_kind
{
	AUTO, BREAK, CASE, CHAR, CONST, CONTINUE, DEFAULT, DO, DOUBLE, ELSE, ENUM, EXTERN, FLOAT, FOR, GOTO,
	IF, INLINE, INT, LONG, REGISTER, RESTRICT, RETURN, SHORT, SIGNED, SIZEOF, STATIC, STRUCT, SWITCH,
	TYPEDEF, UNION, UNSIGNED, VOID, VOLATILE, WHILE,IDENT,INT_CONST
};

extern char token_text[MAXLEN];
extern keyword HashKey[23][7];

int gettoken(FILE * fp);

#endif // !_LEXICAL_H