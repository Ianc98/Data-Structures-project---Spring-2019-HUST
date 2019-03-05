#include "lexer.h"

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

/**********************************************
*函数名称：gettoken
*函数功能：对源代码进行词法分析
*传入参数：源代码文件指针fp
*返回值：单词标识码
**********************************************/
int gettoken(FILE * fp)
{
	char c;							//用于读取字符
	int i = 0; int j = 0;
start:
	/***********************过滤空白**************************/
	while ((c = fgetc(fp)) == ' ' || c == '\n' || c == '\t')
	{
		if (c == '\n') num++;						//如果读取到回车符，行数计数器加1
	}
	
	/***********************判断标识符或关键字**************************/
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

	/***********************判断整型常量或浮点型常量**************************/
	if (c > '0'&&c <= '9')
	{	//如果首字母是数字，单词可能是整型常量或者浮点型常量
		token_text[i] = c;
		i++;
		if ((c = fgetc(fp)) >= '0'&&c <= '9')
		{
		Status4:
			do
			{
				token_text[i] = c;		//将c的值依次放入标识符串	
				i++;
			} while ((c = fgetc(fp)) >= '0'&&c <= '9');		//拼数字串
			if (c == 'u' || c == 'U')
			{
			Status5:
				token_text[i] = c;
				i++;
				if ((c = fgetc(fp)) == 'l' || c == 'L')
				{
					token_text[i] = c;
					i++;
					token_text[i] = 0;
					return UNSIGNED_LONG_CONST;
				}
				else
				{
					ungetc(c, fp);
					token_text[i] = 0;
					return UNSIGNED_CONST;
				}
			}
			else if (c == 'l' || c == 'L')
			{
			Status8:
				token_text[i] = c;
				i++;
				token_text[i] = 0;
				return LONG_CONST;
			}
			else if (c == 'e' || c == 'E')
			{
			Status19:
				token_text[i++] = c;
				if ((c = fgetc(fp)) >= '0'&&c <= '9')
					goto Status10;
				else goto Status17;
			}
			else
			{
			Status9:
				token_text[i] = 0;
				ungetc(c, fp);
				return INT_CONST;
			}
		}
		else if (c == '.')
		{
		Status10:
			do
			{
				token_text[i] = c;		//将c的值依次放入标识符串	
				i++;
			} while ((c = fgetc(fp)) >= '0'&&c <= '9');		//拼数字串(小数部分)
			if (c == 'f' || c == 'F')
			{
				token_text[i] = c;
				i++;
				token_text[i] = 0;
				return FLOAT_CONST;
			}
			else if (c == 'l' || c == 'L')
			{
				token_text[i] = c;
				i++;
				token_text[i] = 0;
				return LONG_DOUBLE_CONST;
			}
			else if (c == 'e' || c == 'E')
				goto Status19;
			else
			{
				ungetc(c, fp);
				return DOUBLE_CONST;
			}
		}
		else if (c == 'u' || c == 'U')
			goto Status5;
		else if (c == 'l' || c == 'L')
			goto Status8;
		else if (c == 'e' || c == 'E')
			goto Status19;
		else if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z') || c == '_')
			goto Status17;
		else goto Status9;
	}

	if (c == '0')
	{
		token_text[i] = c;
		i++;
		if ((c = fgetc(fp)) == 'x' || c == 'X')
		{
			token_text[i] = c;
			i++;
			if (((c = fgetc(fp)) >= '0'&&c <= '9') || (c >= 'A'&&c <= 'Z')
				|| (c >= 'a'&&c <= 'z'))
			{
				do
				{
					token_text[i] = c;
					i++;
				} while (((c = fgetc(fp)) >= '0'&&c <= '9') || (c >= 'A'&&c <= 'Z')
					|| (c >= 'a'&&c <= 'z'));
				if (c == 'u' || c == 'U')
					goto Status5;
				else if (c == 'l' || c == 'L')
					goto Status8;
				else if (c == 'e' || c == 'E')
					goto Status19;
				else goto Status9;
			}
			else
			{
			Status17:
				ungetc(c, fp);
				return ERROR_TOKEN;
			}
		}
		else if (c >= '0'&&c <= '9')
			goto Status4;
		else if (c == 'u' || c == 'U')
			goto Status5;
		else if (c == 'l' || c == 'L')
			goto Status8;
		else if (c == 'e' || c == 'E')
			goto Status19;
		else if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z') || c == '_')
			goto Status17;
		else goto Status9;
	}

	/***********************判断特殊符号**************************/
	switch (c)
	{
	case '.':
		token_text[i++] = c;
		if ((c = fgetc(fp)) >= '0'&&c <= '9')
			goto Status10;
		else goto Status17;
	case '\'':
		token_text[i] = c;
		i++;
		while ((c = fgetc(fp)) != '\'')
		{
			if (c == '\\')
			{
				token_text[i] = c;
				i++;
				c = fgetc(fp);
				token_text[i] = c;
				i++;
				if (c == '\n') goto Status17;
			}
			if (c == '\n' || c == EOF)
				goto Status17;
			token_text[i++] = c;
		}
		token_text[i++] = c;
		token_text[i] = 0;
		return CHAR_CONST;
	case '"':
		token_text[i] = c;
		i++;
		while ((c = fgetc(fp)) != '"')
		{
			if (c == '\\')
			{
				token_text[i] = c;
				i++;
				c = fgetc(fp);
				token_text[i] = c;
				i++;
				if (c == '\n') num++;
			}
			if (c == '\n' || c == EOF)
				goto Status17;
		} 
		token_text[i++] = c;
		token_text[i] = 0;
		return STRING_CONST;
	case '=':
		token_text[i] = c;
		i++;
		c = fgetc(fp);
		if (c == '=')
		{
			token_text[i++] = c;
			token_text[i] = 0;
			return COMPARISON;
		}
		token_text[i] = 0;
		ungetc(c, fp);
		return ASSIGN;
	case '+':
		token_text[i] = c;
		i++;
		c = fgetc(fp);
		if (c == '+') return PLUSPLUS;
		token_text[i] = 0;
		ungetc(c, fp);
		return PLUS;
	case '-':
		token_text[i] = c;
		i++;
		c = fgetc(fp);
		if (c == '-') return MINUSMINUS;
		token_text[i] = 0;
		ungetc(c, fp);
		return MINUS;
	case '*':
		strcpy(token_text, "*");
		return MULTIPLY;
	case '/':
		if ((c = fgetc(fp)) == '/')
		{
			while ((c = fgetc(fp)) != '\n'&&c != EOF);
			if (c == '\n')
			{
				num++;
				goto start;
			}
			else return EOF;
		}
		else if (c == '*')
		{
		Status38:
			do
				c = fgetc(fp);
			while (c != '*'&&c != EOF);
			if (c == EOF) goto Status17;
			else
			{
				if (c = fgetc(fp) == '/')
					goto start;
				else if (c == EOF)
					goto Status17;
				else goto Status38;
			}
		}
		else
		{
			strcpy(token_text, "/");
			ungetc(c, fp);
			return DIVIDE;
		}
	case '%':
		strcpy(token_text, "%");
		return MOD;
	case '>':
		token_text[i++] = '>';
		if ((c = fgetc(fp)) == '=')
		{
			token_text[i++] = c;
			token_text[i] = 0;
			return COMPARISON;
		}
		else
		{
			ungetc(c, fp);
			token_text[i] = 0;
			return COMPARISON;
		}
	case '<':
		token_text[i++] = '<';
		if ((c = fgetc(fp)) == '=')
		{
			token_text[i++] = c;
			token_text[i] = 0;
			return COMPARISON;
		}
		else
		{
			ungetc(c, fp);
			token_text[i] = 0;
			return COMPARISON;
		}
	case '&':
		if (c = fgetc(fp) == '&')
		{
			strcpy(token_text, "&&");
			return AND;
		}
		else goto Status17;
	case '|':
		if (c = fgetc(fp) == '|')
		{
			strcpy(token_text, "||");
			return OR;
		}
		else goto Status17;
	case '(':
		strcpy(token_text, "(");
		return LP;
	case ')':
		strcpy(token_text, ")");
		return RP;
	case '[':
		strcpy(token_text, "[");
		return LBT;
	case ']':
		strcpy(token_text, "]");
		return RBT;
	case '{':
		strcpy(token_text, "{");
		return LBS;
	case '}':
		strcpy(token_text, "}");
		return RBS;
	case ';':
		strcpy(token_text, ";");
		return SEMI;
	case ',':
		strcpy(token_text, ",");
		return COMMA;
	default:
		if (c == EOF) return EOF;
		else return ERROR_TOKEN;		//报错；错误符号
	}
}
