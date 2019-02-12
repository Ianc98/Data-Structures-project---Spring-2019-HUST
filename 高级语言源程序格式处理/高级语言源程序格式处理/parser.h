//
// parser.h
//

#pragma once

#ifndef _PARSER_H
#define _PARSER_H

#include<stdio.h>

typedef int Status;

Status program(FILE * fp);
Status ExtDefList(FILE * fp);

#endif // !_PARSER_H

