//
// parser.h
//

#pragma once

#ifndef _PARSER_H
#define _PARSER_H

#include<stdio.h>
#include"tree.h"

typedef int Status;

CTreePtr program(FILE * fp);
CTreePtr ExtDefList(FILE * fp);
CTreePtr ExtDef(FILE * fp);

#endif // !_PARSER_H

