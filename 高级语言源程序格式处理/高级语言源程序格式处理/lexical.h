//
// lexical.h
//

#pragma once

#ifndef _LEXICAL_H
#define _LEXICAL_H

#include<stdio.h>

enum token_kind
{
	ERROR_TOKEN, IDENT, INT_CONST,
	FLOAT_CONST, CHAR_CONST, INT,
	FLOAT, CHAR, IF, ELSE, EQ, ASSIGN,
	LP, RP, SEMI, COMMA
};
extern char * token_text;
int gettoken(FILE * fp);

#endif // !_LEXICAL_H