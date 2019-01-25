#include "lexical.h"

char token_text[MAXLEN];
keyword HashKey[23][7] = 
{
	{{}},
	{},
	{{"auto",AUTO},{'\0',IDENT}},
{{"break",BREAK},{'\0',IDENT}},
{{"case",CASE},{"char",CHAR},{"const",CONST},{"continue",CONTINUE},{'\0',IDENT}},
{{"default",DEFAULT},{"do",DO},{"double",DOUBLE},{'\0',IDENT}},
{{"else",ELSE},{"enum",ENUM},{"extern",EXTERN},{'\0',IDENT}},
{{"float",FLOAT},{"for",FOR},{'\0',IDENT}},
{{"goto",GOTO},{'\0',IDENT}},
{{'\0',IDENT}},
{{"if",IF},{"inline",INLINE},{"int",INT},{'\0',IDENT}},
{{'\0',IDENT}},
{{'\0',IDENT}},
{{"long",LONG},{'\0',IDENT}},
{{'\0',IDENT}},
{{'\0',IDENT}},
{{'\0',IDENT}},
{{'\0',IDENT}},
{{'\0',IDENT}},
{{"register",REGISTER},{"restrict",RESTRICT},{"return",RETURN},{'\0',IDENT}},
{{"short",SHORT},{"signed",SIGNED},{"sizeof",SIZEOF},{"static",STATIC},
{"struct",STRUCT},{"switch",SWITCH},{'\0',IDENT}},
{{"typedef",TYPEDEF},{'\0',IDENT}},
{{"union",UNION},{"unsigned",UNSIGNED},{'\0',IDENT}},
{{"void",VOID},{"volatile",VOLATILE},{'\0',IDENT}},
{{"while",WHILE},{'\0',IDENT}}
};

int gettoken(FILE * fp)
{
	char c;				//用于读取读取字符
	int i = 0;				//指示在某个数组中的位置
	int j = 0;
	while ((c = fgetc(fp)) == ' ');	//过滤掉空白符号，如果考虑报错位置，对回车需要单独
									//处理，每次回车，设置一个行数计数器加1
	if ((c >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z'))
	{
		do
		{
			token_text[i] = c;		//将c的值依次放入标识符串	
			i++;
		} while (((c = fgetc(fp)) >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z') ||
			(c >= '0'&&c <= '9'));
		//如果c是字母或数字
		ungetc(c, fp);					//退回多读的字符到文件输入缓冲区
		c = token_text[0];
		if (c >= 'A'&&c <= 'Z' || c > 'w') return IDENT;
		i = c - 'a';
		while (strcmp(token_text, HashKey[i][j].key) && HashKey[i][j].key != 0)
			j++;
		return HashKey[i][j].c;
	}
	if (c >= '0'&&c <= '9')
	{
		do
		{
			token_text[i] = c;		//将c的值依次放入标识符串	
			i++;
		} while ((c = fgetc(fp)) >= '0'&&c <= '9');		//拼数字串
		ungetc(c, fp);
		return INT_CONST;
	}
	switch (c)
	{
	default:
		break;
	}
}
