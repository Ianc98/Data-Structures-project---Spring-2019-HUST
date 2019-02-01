#include "lexical.h"

int num = 1;
char token_text[MAXLEN];
keyword HashKey[25][11] = 
{
	{{"_Alignas",_ALIGNAS},{"_Alignof",_ALIGNOF},{"_Atomic",_ATOMIC},{"_Bool",_BOOL},
	{"_Complex",_COMPLEX},{"_Generic",_GENERIC},{"_Imaginary",_IMAGINARY},{"_Noreturn",_NORETURN},
	{"_Static_assert",_STATIC_ASSERT},{"_Thread_local",_THREAD_LOCAL},{"",IDENT}},
	{{"",IDENT}},
	{{"auto",AUTO},{"",IDENT}},
	{{"break",BREAK},{"",IDENT}},
	{{"case",CASE},{"char",CHAR},{"const",CONST},{"continue",CONTINUE},{"",IDENT}},
	{{"default",DEFAULT},{"do",DO},{"double",DOUBLE},{"",IDENT}},
	{{"else",ELSE},{"enum",ENUM},{"extern",EXTERN},{"",IDENT}},
	{{"float",FLOAT},{"for",FOR},{"",IDENT}},
	{{"goto",GOTO},{"",IDENT}},
	{{"",IDENT}},
	{{"if",IF},{"inline",INLINE},{"int",INT},{"",IDENT}},
	{{"",IDENT}},
	{{"",IDENT}},
	{{"long",LONG},{"",IDENT}},
	{{"",IDENT}},
	{{"",IDENT}},
	{{"",IDENT}},
	{{"",IDENT}},
	{{"",IDENT}},
	{{"register",REGISTER},{"restrict",RESTRICT},{"return",RETURN},{"",IDENT}},
	{{"short",SHORT},{"signed",SIGNED},{"sizeof",SIZEOF},{"static",STATIC},
	{"struct",STRUCT},{"switch",SWITCH},{"",IDENT}},
	{{"typedef",TYPEDEF},{"",IDENT}},
	{{"union",UNION},{"unsigned",UNSIGNED},{"",IDENT}},
	{{"void",VOID},{"volatile",VOLATILE},{"",IDENT}},
	{{"while",WHILE},{"",IDENT}}
};
//定义了一个按首字母存放的Hash表，并在每一行末尾设置一个监视哨，标识码为IDENT

int gettoken(FILE * fp)
{
	char c;							//用于读取字符
	int i = 0; int j = 0;
	while ((c = fgetc(fp)) == ' ' || c == '\n')		//过滤掉空白符号
		if (c == '\n') num++;						//如果读取到回车符，行数计数器加1
	if ((c >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z') || c == '_')
	{
		do
		{
			token_text[i] = c;		//将c的值依次放入标识符串	
			i++;
		} while (((c = fgetc(fp)) >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z') ||
			(c >= '0'&&c <= '9') || c == '_');
		//如果c是字母或数字
		token_text[i] = 0;				//结尾插入结束符
		ungetc(c, fp);					//退回多读的字符到文件输入缓冲区
		//标识符可能是关键字，需要判定并返回对应种类码
		//符号串再token_text中，因此查找token_text存储的字符串是否再Hash表中
		//如果标识符和某个关键字相等时，返回关键字的种类编码，否则返回IDENT
		c = token_text[0];
		if (c != '_' && (c<'a' || c>'w')) return IDENT;
		//如果标识符的首字母不为'_'或者'a'-'w'，那么一定时标识符，直接返回IDENT
		i = c - '_';					//查找相应首字母所在的行
		while (strcmp(token_text, HashKey[i][j].key) && *HashKey[i][j].key)
			j++;
		return HashKey[i][j].c;
	}
	if (c >= '0'&&c <= '9')
	{	//如果首字母是数字，单词可能是整型常量或者浮点型常量
		do
		{
			token_text[i] = c;		//将c的值依次放入标识符串	
			i++;
		} while ((c = fgetc(fp)) >= '0'&&c <= '9');		//拼数字串
		if (c == '.')
		{
			do
			{
				token_text[i] = c;		//将c的值依次放入标识符串	
				i++;
			} while ((c = fgetc(fp)) >= '0'&&c <= '9');		//拼数字串(小数部分)
			token_text[i] = 0;
			ungetc(c, fp);
			return FLOAT_CONST;
		}
		token_text[i] = 0;
		ungetc(c, fp);
		return INT_CONST;
	}
	switch (c)
	{
	case '.':
		do
		{
			token_text[i] = c;		//将c的值依次放入标识符串	
			i++;
		} while ((c = fgetc(fp)) >= '0'&&c <= '9');		//拼数字串(小数部分)
		if (i > 1)					//小数点后必须跟随至少1个数字
		{
			token_text[i] = 0;
			ungetc(c, fp);
			return FLOAT_CONST;
		}
		else
		{
			ungetc(c, fp);
			return ERROR_TOKEN;
		}
	case '\'':
	case '"':
		do
		{
			if (c == '\n')
				if (token_text[i - 1] == '\\')
					num++;
				else return ERROR_TOKEN;
			token_text[i] = c;
			i++;
		} while ((c=fgetc(fp))!='"');
		token_text[i++] = c;
		token_text[i] = 0;
		return STRING_CONST;
	case '=':
		c = fgetc(fp);
		if (c == '=') return EQ;
		ungetc(c, fp);
		return ASSIGN;
	case '+':
		c = fgetc(fp);
		if (c == '+') return PLUSPLUS;
		ungetc(c, fp);
		return PLUS;
	case ';':
		return SEMI;
	default:
		if (feof(fp)) return EOF;
		else return ERROR_TOKEN;		//报错；错误符号
	}
}
