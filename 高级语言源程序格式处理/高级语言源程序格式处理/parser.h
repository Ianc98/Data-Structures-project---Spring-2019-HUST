//
// parser.h
//

#pragma once

#ifndef _PARSER_H
#define _PARSER_H

#include<stdio.h>
#include"lexer.h"
#include"queue.h"
#include"tree.h"
#define BEGIN_END -1
#define MAX_OP 10

typedef int Status;

extern int w;						//存取读到的单词
extern char kind[MAXLEN];			//存取类型说明符
extern char tokenText0[MAXLEN];		//存取变量标识符

Status program(FILE * fp, CTree &T);
Status ExtDefList(FILE * fp, CTree &T);
Status ExtDef(FILE * fp, CTree &T);
Status ExtVarDef(FILE * fp, CTree &T);
Status ExtVarList(FILE * fp, CTree &T);
Status funcDef(FILE * fp, CTree &T);
Status ParameterList(FILE * fp, CTree &T);
Status FormParDef(FILE * fp, CTree &T);
Status CompStat(FILE * fp, CTree &T);
Status LocVarList(FILE * fp, CTree &T);
Status LocVarDef(FILE * fp, CTree &T);
Status StatList(FILE * fp, CTree &T);
Status Statement(FILE * fp, CTree &T);
Status exp(FILE * fp, CTree &T, int endsym);
char precede(char * a, char * b);
Status PrintTree(TElemType data, int indent);

#endif // !_PARSER_H

