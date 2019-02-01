#include"lexical.h"
int main(void)
{
	char filename[30]; FILE * fp;
	int status;
	printf("输入文件名：");
	scanf_s("%s", filename, 30);
	if (fopen_s(&fp,filename,"r"))
	{
		printf("文件打开失败！\n");
		getchar(); return 0;
	}
	printf("\n");
	printf("	单词类别		单词值\n");
	while (!feof(fp))
	{
		status = gettoken(fp);
		if (status >= AUTO && status <= _THREAD_LOCAL)
			printf("	关键字			%s\n", token_text);
		switch (status)
		{
		case IDENT:
			printf("	标识符			%s\n", token_text);
			break;
		case INT_CONST:
			printf("	整型常量		%s\n", token_text);
			break;
		case FLOAT_CONST:
			printf("	浮点型常量		%s\n", token_text);
			break;
		case EQ:
			printf("	等号			==\n");
			break;
		case ASSIGN:
			printf("	赋值号			=\n");
			break;
		case PLUS:
			printf("	加号			+\n");
			break;
		case PLUSPLUS:
			printf("	自增号			++\n");
			break;
		case SEMI:
			printf("	分号			;\n");
			break;
		case ERROR_TOKEN:
			printf("\n");
			printf("出错！出错行数：%d\n", num);
			printf("\n");
			printf("按任意键继续...");
			getchar(); getchar();
			return 0;
		}
	}
	printf("\n");
	printf("按任意键继续...");
	getchar(); getchar();
	return 1;
}