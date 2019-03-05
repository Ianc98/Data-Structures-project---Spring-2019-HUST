#include"parser.h"
#include"stack.h"
#include"queue.h"
#include"tree.h"

int w;
char kind[MAXLEN];
char tokenText0[MAXLEN];
int indent0 = 0;
queue<print> printList;

/***********************************************************
*函数名称：program
*函数功能：分析当前语法成分是否为程序
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status program(FILE * fp, CTree &T)
{
	CTree c;
	print elem = { indent0,num };
	printList.push(elem);
	w = gettoken(fp);
	if (!ExtDefList(fp, c)) return ERROR;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("程序") + 1) * sizeof(char));		//定义一个结点作为空树的根
	strcpy(T.nodes[0].data, "程序");		//定义一个结点作为空树的根
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;
	InsertChild(T, T.r, 1, c);
	return OK;
}

/***********************************************************
*函数名称：ExtDefList
*函数功能：分析当前语法成分是否为外部定义序列
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status ExtDefList(FILE * fp, CTree &T)
{
	CTree c; Status p;
	if (w == EOF) return INFEASIBLE;
	T.n = 1;  T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("外部定义序列") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "外部定义序列");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;
	if (!ExtDef(fp, c)) return ERROR;
	InsertChild(T, T.r, 1, c);
	p = ExtDefList(fp, c);
	if (p == OK) InsertChild(T, T.r, 2, c);
	if (!p) return ERROR;
	return OK;
}

/***********************************************************
*函数名称：ExtDef
*函数功能：分析当前语法成分是否为外部定义
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status ExtDef(FILE * fp, CTree &T)
{	
	Status p;
	if (w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED &&
		w != FLOAT && w != DOUBLE && w != CHAR) return ERROR;
	strcpy(kind, token_text);			//保存类型说明符
	w = gettoken(fp);
	if (w != IDENT) return ERROR;
	strcpy(tokenText0, token_text);		//保存第一个变量名或函数名到tokenText0
	w = gettoken(fp);
	if (w != LP) p = ExtVarDef(fp, T);	//调用外部变量定义子程序
	else p = funcDef(fp, T);			//调用函数定义子程序
	if (!p) return ERROR;
	return OK;
}

/***********************************************************
*函数名称：ExtVarDef
*函数功能：分析当前语法成分是否为外部变量定义
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status ExtVarDef(FILE * fp, CTree & T)
{
	CTree c; CTree p;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("外部变量定义：")+1)*sizeof(char));
	strcpy(T.nodes[0].data, "外部变量定义：");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;						//生成外部变量定义结点
	c.n = 1; c.r = 0;
	c.nodes[0].data = (char *)malloc((strlen(kind)+strlen("类型：") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "类型：");
	strcat(c.nodes[0].data, kind);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;						//生成外部变量类型结点
	if (!InsertChild(T, T.r, 1, c)) return ERROR;		//c作为root的第一个孩子
	if (!ExtVarList(fp, p)) return ERROR;
	if (!InsertChild(T, T.r, 2, p)) return ERROR;		//p作为root的第二个孩子
	return OK;
}

/***********************************************************
*函数名称：ExtVarList
*函数功能：分析当前语法成分是否为外部变量序列
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status ExtVarList(FILE * fp, CTree & T)
{//初始时，tokenText0保存了第一个变量名
	CTree c; CTree t;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("外部变量序列") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "外部变量序列");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;				//生成外部变量序列结点
	if (w == LBT)
	{
		strcat(tokenText0, "[");
		if ((w = gettoken(fp)) == INT_CONST)
		{
			strcat(tokenText0, token_text);
			if ((w = gettoken(fp)) == RBT)
			{
				strcat(tokenText0, "]");
				c.n = 1; c.r = 0;
				c.nodes[0].data = (char *)malloc((strlen(tokenText0) + strlen("Array: ") + 1) * sizeof(char));
				strcpy(c.nodes[0].data, "Array: ");
				strcat(c.nodes[0].data, tokenText0);
				c.nodes[0].indent = 1;
				c.nodes[0].firstchild = NULL;
				w = gettoken(fp);
			}
			else return ERROR;
		}
		else return ERROR;
	}
	else
	{
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char *)malloc((strlen(tokenText0) + strlen("ID: ") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "ID: ");
		strcat(c.nodes[0].data, tokenText0);
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
	}
	if (!InsertChild(T, T.r, 1, c))	return ERROR;//tokenText0作为root的第一个孩子
	if (w != COMMA && w != SEMI) return ERROR;
	if (w == SEMI)								//如果标识符后是分号，直接结束
	{
		w = gettoken(fp);
		return OK;
	}
	w = gettoken(fp);
	if (w != IDENT) return ERROR;
	//否则后面应当还是标识符
	strcpy(tokenText0, token_text);
	w = gettoken(fp);
	if (!ExtVarList(fp, t)) return ERROR;
	if(!InsertChild(T,T.r,2,t)) return ERROR;
	return OK;
}

/***********************************************************
*函数名称：funcDef
*函数功能：分析当前语法成分是否为函数定义
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status funcDef(FILE * fp, CTree & T)
{
	CTree c; CTree p; CTree q; CTree f; CTree s;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("函数定义：") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "函数定义：");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;		//生成函数定义结点root
	c.n = 1; c.r = 0;
	c.nodes[0].data = (char *)malloc((strlen(kind) + strlen("类型：") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "类型：");
	strcat(c.nodes[0].data, kind);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;		//生成返回值类型结点
	if (!InsertChild(T, T.r, 1, c)) return ERROR;
	w = gettoken(fp);
	//函数括号内可能无参数，可能是void，可能是参数序列
	//其他情况报错
	if (w != RP && w != VOID && w != INT && w != LONG && w != SHORT &&
		w != SIGNED && w != UNSIGNED && w != FLOAT && w != DOUBLE && w != CHAR)
		return ERROR;
	p.n = 1; p.r = 0;
	p.nodes[0].data = (char *)malloc((strlen(tokenText0) + strlen("函数名：") + 1) * sizeof(char));
	strcpy(p.nodes[0].data, "函数名：");
	strcat(p.nodes[0].data, tokenText0);
	p.nodes[0].indent = 1;
	p.nodes[0].firstchild = NULL;		//生成函数名结点
	if (w == RP||w==VOID)
	{
		if (w == VOID)
		{
			w = gettoken(fp);
			if (w != RP) return ERROR;
		}
	}
	else
	{
		if (!ParameterList(fp, q)) return ERROR;
		if (!InsertChild(p, p.r, 1, q)) return ERROR;
	}
	if (!InsertChild(T, T.r, 2, p)) return ERROR;
	w = gettoken(fp);
	if (w != SEMI && w != LBS) return ERROR;
	f.n = 1; f.r = 0;
	f.nodes[0].data = (char *)malloc((strlen("函数体：") + 1) * sizeof(char));
	strcpy(f.nodes[0].data, "函数体：");
	f.nodes[0].indent = 1;
	f.nodes[0].firstchild = NULL;		//生成函数体结点
	if (w == LBS)
		if (!CompStat(fp, s)) return ERROR;
	if (!InsertChild(f, f.r, 1, s)) return ERROR;
	InsertChild(T, T.r, 3, f);
	return OK;
}

/***********************************************************
*函数名称：ParameterList
*函数功能：分析当前语法成分是否为参数序列
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status ParameterList(FILE * fp, CTree & T)
{
	CTree c;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("形参序列") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "形参序列");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;				//生成形参序列结点
	if (!FormParDef(fp, c)) return ERROR;
	if (!InsertChild(T, T.r, 1, c)) return ERROR;
	w = gettoken(fp);
	if (w != RP && w != COMMA) return ERROR;
	if (w == COMMA)
	{
		w = gettoken(fp);
		if (!ParameterList(fp, c)) return ERROR;
		InsertChild(T, T.r, 2, c);
	}
	return OK;
}

/***********************************************************
*函数名称：FormParDef
*函数功能：分析当前语法成分是否为形式参数，生成的树
保存在CTree T中
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status FormParDef(FILE * fp, CTree & T)
{
	CTree c;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("形参：") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "形参：");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;		//生成形参结点
	if (w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED &&
		w != FLOAT && w != DOUBLE && w != CHAR) return ERROR;
	c.n = 1; c.r = 0;
	c.nodes[0].data = (char *)malloc((strlen(token_text) + strlen("类型：") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "类型：");
	strcat(c.nodes[0].data, token_text);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;		//生成形参类型结点
	InsertChild(T, T.r, 1, c);
	w = gettoken(fp);
	if (w != IDENT) return ERROR;
	c.nodes[0].data = (char *)malloc((strlen(token_text) + strlen("ID: ") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "ID: ");
	strcat(c.nodes[0].data, token_text);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;
	InsertChild(T, T.r, 2, c);
	return OK;
}

/***********************************************************
*函数名称：CompStat
*函数功能：分析当前语法成分是否为复合语句
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status CompStat(FILE * fp, CTree & T)
{
	CTree c; CTree p;
	Status status;
	print elem;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("复合语句：")+1)*sizeof(char));
	strcpy(T.nodes[0].data, "复合语句：");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;		//生成复合语句结点
	//注意其中局部变量说明和语句序列都可以为空
	w = gettoken(fp);
	elem = { ++indent0,num };
	printList.push(elem);
	if (w == INT || w == LONG || w == SHORT || w == SIGNED || w == UNSIGNED
		|| w == FLOAT || w == DOUBLE || w == CHAR)
	{
		if (!LocVarList(fp, c)) return ERROR;
		if(!InsertChild(T,T.r,1,c)) return ERROR;
		status = StatList(fp, p);
		if (!status) return ERROR;
		if(status==OK)
			if (!InsertChild(T, T.r, 2, p)) return ERROR;
	}
	else
	{
		status = StatList(fp, p);
		if (!status) return ERROR;
		if (status == OK)
			if (!InsertChild(T, T.r, 1, p)) return ERROR;
	}
	elem = { --indent0,num };
	printList.push(elem);
	if (w != RBS) return ERROR;
	w = gettoken(fp);
	return OK;
}

/***********************************************************
*函数名称：LocVarList
*函数功能：分析当前语法成分是否为局部变量定义序列
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status LocVarList(FILE * fp, CTree & T)
{
	CTree c; Status p;
	if (w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED &&
		w != FLOAT && w != DOUBLE && w != CHAR)
		return INFEASIBLE;						
	//读到的后继单词不为类型说明符时，变量定义序列结束
	T.n = 1;  T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("局部变量定义序列") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "局部变量定义序列");
	T.nodes[0].indent = 0;
	T.nodes[0].firstchild = NULL;		//生成局部变量定义序列结点
	if (!LocVarDef(fp, c)) return ERROR;
	if (!InsertChild(T, T.r, 1, c)) return ERROR;
	w = gettoken(fp);
	p = LocVarList(fp, c);
	if (p == OK) InsertChild(T, T.r, 2, c);
	if (!p) return ERROR;
	return OK;
}

/***********************************************************
*函数名称：LocVarDef
*函数功能：分析当前语法成分是否为局部定义
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status LocVarDef(FILE * fp, CTree & T)
{
	CTree c;  CTree t; int i = 1;
	CTree *Treeptr;
	if (w != INT && w != LONG && w != SHORT && w != SIGNED && w != UNSIGNED &&
		w != FLOAT && w != DOUBLE && w != CHAR) return ERROR;
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("局部变量定义：") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "局部变量定义：");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;			//生成局部变量定义结点
	c.n = 1; c.r = 0;
	c.nodes[0].data = (char *)malloc((strlen(token_text) + strlen("类型：") + 1) * sizeof(char));
	strcpy(c.nodes[0].data, "类型：");
	strcat(c.nodes[0].data, token_text);
	c.nodes[0].indent = 1;
	c.nodes[0].firstchild = NULL;			//生成变量类型结点
	if (!InsertChild(T, T.r, 1, c)) return ERROR;
	t.n = 1; t.r = 0;
	t.nodes[0].data = (char *)malloc((strlen("变量名序列") + 1) * sizeof(char));
	strcpy(t.nodes[0].data, "变量名序列");
	t.nodes[0].indent = 0;
	t.nodes[0].firstchild = NULL;
	do
	{
		w = gettoken(fp); strcpy(tokenText0, token_text);
		if (w != IDENT) return ERROR;
		Treeptr = (CTree *)malloc(sizeof(CTree));
		if (!Treeptr) exit(OVERFLOW);
		w = gettoken(fp);
		if (w == LBT)
		{
			strcat(tokenText0, "[");
			if ((w = gettoken(fp)) == INT_CONST)
			{
				strcat(tokenText0, token_text);
				if ((w = gettoken(fp)) == RBT)
				{
					strcat(tokenText0, "]");
					Treeptr->n = 1; Treeptr->r = 0;
					Treeptr->nodes[0].data = (char *)malloc((strlen(tokenText0) + strlen("Array: ") + 1) * sizeof(char));
					strcpy(Treeptr->nodes[0].data, "Array: ");
					strcat(Treeptr->nodes[0].data, tokenText0);
					Treeptr->nodes[0].indent = 1;
					Treeptr->nodes[0].firstchild = NULL;
					w = gettoken(fp);
				}
				else return ERROR;
			}
			else return ERROR;
		}
		else
		{
			Treeptr->n = 1; Treeptr->r = 0;
			Treeptr->nodes[0].data = (char *)malloc((strlen(tokenText0) + strlen("ID: ") + 1) * sizeof(char));
			strcpy(Treeptr->nodes[0].data, "ID: ");
			strcat(Treeptr->nodes[0].data, tokenText0);
			Treeptr->nodes[0].indent = 1;
			Treeptr->nodes[0].firstchild = NULL;		//生成变量名结点
		}
		if (!InsertChild(t, t.r, i++, *Treeptr)) return ERROR;
	} while (w == COMMA);
	if (w != SEMI) return ERROR;
	if (!InsertChild(T, T.r, 2, t)) return ERROR;
	return OK;
}

/***********************************************************
*函数名称：StatList
*函数功能：分析当前语法成分是否为语句序列
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status StatList(FILE * fp, CTree & T)
{
	Status status; CTree c, t;
	status = Statement(fp, c);
	if (status == INFEASIBLE) return INFEASIBLE;
	if (!status) return ERROR;
	else
	{
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char *)malloc((strlen("语句序列") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "语句序列");
		T.nodes[0].indent = 0;
		T.nodes[0].firstchild = NULL;		//生成语句序列结点
		InsertChild(T, T.r, 1, c);
		status = StatList(fp, t);
		if (!status) return ERROR;
		if (status == OK)
			if (!InsertChild(T, T.r, 2, t))
				return ERROR;
	}
	return OK;
}

/***********************************************************
*函数名称：Statement
*函数功能：分析当前语法成分是否为语句
*传入参数：源文件指针fp,树CTree T
*返回值：返回函数执行状态
***********************************************************/
Status Statement(FILE * fp, CTree & T)
{
	CTree c; CTree p; CTree q; CTree k;
	print elem;
	switch (w)
	{
	case IF://分析条件语句
		w = gettoken(fp);
		if (w != LP) return ERROR;
		w = gettoken(fp);
		if (w == RP) return ERROR;
		if (!exp(fp, p, RP)) return ERROR;
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char *)malloc((strlen("条件：") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "条件：");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(c, c.r, 1, p);
		w = gettoken(fp);
		if (w == LBS)
		{
			if (!CompStat(fp, q)) return ERROR;
		}
		else
		{
			elem = { ++indent0,num };
			printList.push(elem);
			if (!Statement(fp, q)) return ERROR;
			elem = { --indent0,num };
			printList.push(elem);
		}
		p.n = 1; p.r = 0;
		p.nodes[0].data = (char *)malloc((strlen("IF子句：") + 1) * sizeof(char));
		strcpy(p.nodes[0].data, "IF子句：");
		p.nodes[0].indent = 1;
		p.nodes[0].firstchild = NULL;
		InsertChild(p, p.r, 1, q);
		if (w == ELSE)
		{
			w = gettoken(fp);
			if (w == LBS)
			{
				if (!CompStat(fp, k)) return ERROR;
			}
			else
			{
				elem = { ++indent0,num };
				printList.push(elem);
				if (!Statement(fp, k)) return ERROR;
				elem = { --indent0,num };
				printList.push(elem);
			}
			q.n = 1; q.r = 0;
			q.nodes[0].data = (char *)malloc((strlen("ELSE子句：") + 1) * sizeof(char));
			strcpy(q.nodes[0].data, "ELSE子句：");
			q.nodes[0].indent = 1;
			q.nodes[0].firstchild = NULL;
			InsertChild(q, q.r, 1, k);
			T.n = 1; T.r = 0;
			T.nodes[0].data = (char *)malloc((strlen("if-else语句：") + 1) * sizeof(char));
			strcpy(T.nodes[0].data, "if-else语句：");
			T.nodes[0].indent = 1;
			T.nodes[0].firstchild = NULL;
			InsertChild(T, T.r, 1, c);
			InsertChild(T, T.r, 2, p);
			InsertChild(T, T.r, 3, q);
		}
		else
		{
			T.n = 1; T.r = 0;
			T.nodes[0].data = (char *)malloc((strlen("if语句：") + 1) * sizeof(char));
			strcpy(T.nodes[0].data, "if语句：");
			T.nodes[0].indent = 1;
			T.nodes[0].firstchild = NULL;
			InsertChild(T, T.r, 1, c);
			InsertChild(T, T.r, 2, p);
		}
		return OK;
	case LBS:
		if (!CompStat(fp, T)) return ERROR;
		return OK;
	case FOR:
		//分析for语句
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char *)malloc((strlen("for语句：") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "for语句：");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char *)malloc((strlen("初始表达式：") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "初始表达式：");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 1, c);
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char *)malloc((strlen("终止条件：") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "终止条件：");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 2, c);
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char *)malloc((strlen("循环表达式：") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "循环表达式：");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 3, c);
		c.n = 1; c.r = 0;
		c.nodes[0].data = (char *)malloc((strlen("for子句：") + 1) * sizeof(char));
		strcpy(c.nodes[0].data, "for子句：");
		c.nodes[0].indent = 1;
		c.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 4, c);
		w = gettoken(fp);
		if (w != LP) return ERROR;
		w = gettoken(fp);
		if (!exp(fp, c, SEMI)) return ERROR;
		InsertChild(T, T.nodes[0].firstchild->child, 1, c);
		w = gettoken(fp);
		if (w == SEMI) return ERROR;
		if (!exp(fp, c, SEMI)) return ERROR;
		InsertChild(T, T.nodes[0].firstchild->next->child, 1, c);
		w = gettoken(fp);
		if (!exp(fp, c, RP)) return ERROR;
		InsertChild(T, T.nodes[0].firstchild->next->next->child, 1, c);
		w = gettoken(fp);
		if (w == LBS)
		{
			if (!CompStat(fp, c)) return ERROR;
		}
		else
		{
			elem = { ++indent0,num };
			printList.push(elem);
			if (!Statement(fp, c)) return ERROR;
			elem = { --indent0,num };
			printList.push(elem);
		}
		InsertChild(T, T.nodes[0].firstchild->next->next->next->child, 1, c);
		return OK;
	case WHILE:
		w = gettoken(fp);
		if (w != LP) return ERROR;
		w = gettoken(fp);
		if (w == RP) return ERROR;
		if (!exp(fp, c, RP)) return ERROR;
		w = gettoken(fp);
		if (w == LBS)
		{
			if (!CompStat(fp, p)) return ERROR;
		}
		else
		{
			elem = { ++indent0,num };
			printList.push(elem);
			if (!Statement(fp, p)) return ERROR;
			elem = { --indent0,num };
			printList.push(elem);
		}
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char *)malloc((strlen("while语句：") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "while语句：");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		InsertChild(T, T.r, 1, c);
		InsertChild(T, T.r, 2, p);
		return OK;
	case DO:
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char *)malloc((strlen("do-while语句：") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "do-while语句：");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		if (w == LBS)
		{
			if (!CompStat(fp, c)) return ERROR;
		}
		else
		{
			elem = { ++indent0,num };
			printList.push(elem);
			if (!Statement(fp, c)) return ERROR;
			elem = { --indent0,num };
			printList.push(elem);
		}
		InsertChild(T, T.r, 1, c);
		if (w != WHILE) return ERROR;
		w = gettoken(fp);
		if (w != LP) return ERROR;
		w = gettoken(fp);
		if (w == RP) return ERROR;
		if (!exp(fp, c, RP)) return ERROR;
		InsertChild(T, T.r, 2, c);
		w = gettoken(fp);
		if (w != SEMI) return ERROR;
		w = gettoken(fp);
		return OK;
	case CONTINUE:
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char *)malloc((strlen("continue语句：") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "continue语句：");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		if (w != SEMI) return ERROR;
		w = gettoken(fp);
		return OK;
	case BREAK:
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char *)malloc((strlen("break语句：") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "break语句：");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		if (w != SEMI) return ERROR;
		w = gettoken(fp);
		return OK;
	case RETURN:
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char *)malloc((strlen("return语句：") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "return语句：");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		if (w == SEMI) return ERROR;
		if (!exp(fp, c, SEMI)) return ERROR;
		w = gettoken(fp);
		InsertChild(T, T.r, 1, c);
		return OK;
	case LP:
		if (!exp(fp, T, RP)) return ERROR;
		w = gettoken(fp);
		return OK;
	case IDENT:
	case INT_CONST:
	case UNSIGNED_CONST:
	case LONG_CONST:
	case UNSIGNED_LONG_CONST:
	case DOUBLE_CONST:
	case FLOAT_CONST:
	case LONG_DOUBLE_CONST:
	case CHAR_CONST:
		if (!exp(fp, T, SEMI)) return ERROR;
		w = gettoken(fp);
		return OK;
	case RBS:
		return INFEASIBLE;
	case SEMI: 
		T.n = 1; T.r = 0;
		T.nodes[0].data = (char *)malloc((strlen("空语句") + 1) * sizeof(char));
		strcpy(T.nodes[0].data, "空语句");
		T.nodes[0].indent = 1;
		T.nodes[0].firstchild = NULL;
		w = gettoken(fp);
		return OK;
	default:
		return ERROR;
	}
}

/***********************************************************
*函数名称：exp
*函数功能：分析当前语法成分是否为表达式
*传入参数：源文件指针fp,树CTree T,表达式结束符号int endsym
*返回值：返回函数执行状态
***********************************************************/
Status exp(FILE * fp, CTree & T, int endsym)
{
	//调用该算法时，在调用者已经读入了第一个单词在w中
	SqStack op;		//运算符栈
	SqStack opn;	//操作数栈
	CTree * node = (CTree *)malloc(sizeof(CTree));
	CTree * child1 = (CTree *)malloc(sizeof(CTree));
	CTree * child2 = (CTree *)malloc(sizeof(CTree));
	T.n = 1; T.r = 0;
	T.nodes[0].data = (char *)malloc((strlen("表达式语句：") + 1) * sizeof(char));
	strcpy(T.nodes[0].data, "表达式语句：");
	T.nodes[0].indent = 1;
	T.nodes[0].firstchild = NULL;
	int error = 0;
	node->n = 1; node->r = 0;
	node->nodes[0].data = (char *)malloc((strlen("#") + 1) * sizeof(char));
	strcpy(node->nodes[0].data, "#");
	node->nodes[0].firstchild = NULL;
	InitStack(opn);  InitStack(op);  Push(op, node);		//初始化，将起止符#入栈
	while ((w != BEGIN_END || strcmp(node->nodes[0].data, "#")) && !error)  //当运算符栈栈顶不是起止符号，并没有错误时
	{
		if (w == IDENT || w == INT_CONST || w == UNSIGNED_CONST || w == LONG_CONST
			|| w == UNSIGNED_LONG_CONST || w == DOUBLE_CONST || w == FLOAT_CONST
			|| w == LONG_DOUBLE_CONST || w == CHAR_CONST)
		{
			node = (CTree *)malloc(sizeof(CTree));
			node->n = 1; node->r = 0;
			node->nodes[0].data = (char *)malloc((strlen(token_text) + strlen("ID: ") + 1) * sizeof(char));
			strcpy(node->nodes[0].data, "ID: ");
			strcat(node->nodes[0].data, token_text);
			node->nodes[0].indent = 1;
			node->nodes[0].firstchild = NULL;
			Push(opn, node);								//根据w生成一个结点，结点指针进栈opn
			w = gettoken(fp);
		}
		else if (w == PLUS || w == MINUS || w == MULTIPLY || w == DIVIDE ||
			w == MOD || w == LP || w == RP || w == COMPARISON || w == ASSIGN ||
			w == AND || w == OR || w == BEGIN_END)
		{
			node = (CTree *)malloc(sizeof(CTree));
			GetTop(op, node);
			if (w == BEGIN_END) strcpy(token_text, "#");
			switch (precede(node->nodes[0].data, token_text))
			{
			case '<':
				node = (CTree *)malloc(sizeof(CTree));
				node->nodes[0].data = (char *)malloc((strlen(token_text) + 1) * sizeof(char));
				strcpy(node->nodes[0].data, token_text);
				node->nodes[0].indent = 1;
				node->nodes[0].firstchild = NULL;
				node->n = 1; node->r = 0;
				Push(op, node);		//根据w生成一个结点，结点指针进栈opn
				w = gettoken(fp);
				break;
			case '=':
				if (!Pop(op, node)) error++;
				w = gettoken(fp);
				break;   //去括号
			case '>':if (!Pop(opn, child2)) error++;
				if (!Pop(opn, child1)) error++;
				if (!Pop(op, node)) error++;
				//根据运算符栈退栈得到的运算符t和操作数的结点指针t1和t2，
				//完成建立生成一个运算符的结点，结点指针进栈opn
				InsertChild(*node, node->r, 1, *child1);
				InsertChild(*node, node->r, 2, *child2);
				Push(opn, node);
				break;
			default:
				if (w == endsym) w = BEGIN_END; //遇到结束标记），w被替换成#
				else error++;
			}
		}
		else if (w == endsym) w = BEGIN_END;//遇到结束标记分号，w被替换成#
		else error = 1;
		GetTop(op, node);
	}
	if (error) return ERROR;
	GetTop(opn, node);
	InsertChild(T, T.r, 1, *node);
	return OK;
}


char precede(char * a, char * b)
{
	int i, j;		//指示运算符对应的编号
	//定义一个二维数组，用于存放优先级
	char precede[MAX_OP][MAX_OP]=
	{//				+		-		*		/		%		（		）		=		>和<		==和!=		#		&&		||
	/* + */			'>',	'>',	'<',	'<',	'<',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
	/* - */			'>',	'>',	'<',	'<',	'<',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
	/* * */			'>',	'>',	'>',	'>',	'>',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
	/* / */			'>',	'>',	'>',	'>',	'>',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
	/* % */			'>',	'>',	'<',	'<',	'<',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
	/* ( */			'<',	'<',	'<',	'<',	'<',	'<',	'=',	'?',	'<',		'<',		'>',	'<',	'<',
	/* ) */			'>',	'>',	'>',	'>',	'>',	'>',	'?',	'?',	'>',		'>',		'>',	'>',	'>',
	/* = */			'<',	'<',	'<',	'<',	'<',	'<',	'?',	'<',	'<',		'<',		'>',	'<',	'<',
	/* >和< */		'<',	'<',	'<',	'<',	'<',	'<',	'>',	'?',	'>',		'>',		'>',	'>',	'>',
	/* ==和!= */	'<',	'<',	'<',	'<',	'<',	'<',	'>',	'?',	'<',		'>',		'>',	'>',	'>',
	/* # */			'<',	'<',	'<',	'<',	'<',	'<',	'?',	'<',	'<',		'<',		'=',	'<',	'<',
	/* && */		'<',	'<',	'<',	'<',	'<',	'<',	'>',	'>',	'<',		'<',		'>',	'>',	'>',
	/* || */		'<',	'<',	'<',	'<',	'<',	'<',	'>',	'>',	'<',		'<',		'>',	'>',	'>'
	};
	switch (a[0])
	{
	case '+':
		i = 0; break;
	case '-':
		i = 1; break;
	case '*':
		i = 2; break;
	case '/':
		i = 3; break;
	case '%':
		i = 4; break;
	case '(':
		i = 5; break;
	case ')':
		i = 6; break;
	case '=':
		if (a[1] == '=') i = 9;
		else i = 7;
		break;
	case '>':
	case '<':
		i = 8; break;
	case '!':
		if (a[1] == '=') i = 9;
		else return '?';
		break;
	case '#':
		i = 10;
		break;
	case '&':
		if (a[1] == '&') i = 11;
		else return '?';
		break;
	case '|':
		if (a[1] == '|') i = 12;
		else return '?';
		break;
	default:
		return '?';
	}
	switch (b[0])
	{
	case '+':
		j = 0; break;
	case '-':
		j = 1; break;
	case '*':
		j = 2; break;
	case '/':
		j = 3; break;
	case '%':
		j = 4; break;
	case '(':
		j = 5; break;
	case ')':
		j = 6; break;
	case '=':
		if (b[1] == '=') j = 9;
		else j = 7;
		break;
	case '>':
	case '<':
		j = 8; break;
	case '!':
		if (b[1] == '=') j = 9;
		else return '?';
		break;
	case '#':
		j = 10;
		break;
	case '&':
		if (b[1] == '&') j = 11;
		else return '?';
		break;
	case '|':
		if (b[1] == '|') j = 12;
		else return '?';
		break;
	default:
		return '?';
	}
	return precede[i][j];
}

Status PrintTree(TElemType data, int indent)
{
	int i;
	for (i = 0; i < indent; i++)
		printf("\t");
	printf("%s\n", data);
	return OK;
}
