#include"lexer.h"
#include"parser.h"
#include"queue.h"
#include"tree.h"
#include"Print_CFile.h"
#include"pre_process.h"

//对队列所用的visit()函数进行定义
Status PrintElement(QElemType e)
{
	int i = 1;					//标记错误序号
	printf("	%d		%d\n", i++, e);
	return OK;
}

int main(void)
{
	char filename[30]; FILE * fp, * fp1;
	int status; int ch = 1;
	LinkQueue Q;				//构建一个队列，用于存放错误行
	CTree T;
	InitQueue(Q);
ChooseFilename:
	system("cls");
	printf("输入文件名：");
	scanf_s("%s", filename, 30);
	while (ch)
	{
		if (fopen_s(&fp, filename, "r"))
		{
			printf("文件打开失败！\n");
			printf("按任意键继续...");
			getchar(); getchar();
			goto ChooseFilename;
		}
		system("cls");
		printf("文件名：%s\n\n", filename);
		printf("高级语言源程序格式处理菜单\n");
		printf("1. 词法分析\n");
		printf("2. 语法分析\n");
		printf("3. 格式化源文件\n");
		printf("4. 重新选择文件\n");
		printf("0. 退出程序\n");
		printf("\n");
		printf("请进行选择：");
		scanf("%d", &ch);
		system("cls");
		switch (ch)
		{
		case 1:
			printf("正在预编译文件...\n");
			if (fp = pre_process(fp))
			{
				printf("预编译成功！生成中间文件mid_cpp\n");
				printf("按任意键继续...\n");
				getchar(); getchar();
			}
			else
			{
				printf("预编译失败！请检查错误！\n");
				printf("按任意键继续...");
				getchar(); getchar();
				return 0;
			}
			fp = fopen("mid_cpp", "r");
			system("cls");
			printf("\n");
			printf("	单词类别				单词值\n");
			while (!feof(fp))
			{
				status = gettoken(fp);
				if (status >= AUTO && status <= _THREAD_LOCAL)
					printf("	关键字					%s\n", token_text);
				switch (status)
				{
				case IDENT:
					printf("	标识符					%s\n", token_text);
					break;
				case INT_CONST:
					printf("	整型常量				%s\n", token_text);
					break;
				case UNSIGNED_CONST:
					printf("	无符号整型常量				%s\n", token_text);
					break;
				case LONG_CONST:
					printf("	长整型常量				%s\n", token_text);
					break;
				case UNSIGNED_LONG_CONST:
					printf("	无符号长整型常量			%s\n", token_text);
					break;
				case DOUBLE_CONST:
					printf("	双精度浮点型常量			%s\n", token_text);
					break;
				case FLOAT_CONST:
					printf("	浮点型常量				%s\n", token_text);
					break;
				case LONG_DOUBLE_CONST:
					printf("	长双精度浮点型常量			%s\n", token_text);
					break;
				case CHAR_CONST:
					printf("	字符型常量				%s\n", token_text);
					break;
				case STRING_CONST:
					printf("	字符串常量				%s\n", token_text);
					break;
				case ASSIGN:
					printf("	赋值运算符				=\n");
					break;
				case PLUS:
					printf("	加法运算符				+\n");
					break;
				case PLUSPLUS:
					printf("	自增运算符				++\n");
					break;
				case MINUS:
					printf("	减法运算符				-\n");
					break;
				case MINUSMINUS:
					printf("	自减运算符				--\n");
					break;
				case MULTIPLY:
					printf("	乘法运算符				*\n");
					break;
				case DIVIDE:
					printf("	除法运算符				/\n");
					break;
				case MOD:
					printf("	取模运算符				%%\n");
					break;
				case COMPARISON:
					printf("	关系运算符				%s\n", token_text);
					break;
				case AND:
					printf("	逻辑与					&&\n");
					break;
				case OR:
					printf("	逻辑或					||\n");
					break;
				case LP:
					printf("	左小括号				(\n");
					break;
				case RP:
					printf("	右小括号				)\n");
					break;
				case LBT:
					printf("	左中括号				[\n");
					break;
				case RBT:
					printf("	右中括号				]\n");
					break;
				case LBS:
					printf("	左大括号				{\n");
					break;
				case RBS:
					printf("	右大括号				}\n");
					break;
				case SEMI:
					printf("	分号					;\n");
					break;
				case COMMA:
					printf("	逗号					,\n");
					break;
				case ERROR_TOKEN:
					EnQueue(Q, num);
					break;
				}
			}
			printf("\n");
			printf("错误列表 %d\n", QueueLength(Q));
			if (QueueLength(Q))
			{
				printf("\n");
				printf("	序号		行\n");
				QueueTraverse(Q, PrintElement);
			}
			fclose(fp);
			printf("\n按任意键继续...");
			getchar();
			num = 1;
			break;
		case 2:
			printf("正在预编译文件...\n");
			if (fp = pre_process(fp))
			{
				printf("预编译成功！生成中间文件mid_cpp\n");
				printf("按任意键继续...\n");
				getchar(); getchar();
			}
			else
			{
				printf("预编译失败！请检查错误！\n");
				printf("按任意键继续...");
				getchar(); getchar();
				return 0;
			}
			fp = fopen("mid_cpp", "r");
			system("cls");
			if (!program(fp, T))
			{
				printf("发生生成错误！错误行号：%d\n", num);
				printf("按任意键继续...\n");
				getchar(); num = 1;
				break;
			}
			TraverseTree(T, PrintTree);
			printf("\n按任意键继续...\n");
			getchar();
			num = 1;
			break;
		case 3:
			if (!pre_process(fp))
			{
				printf("程序预编译失败！\n");
				printf("按任意键继续...");
				getchar(); getchar();
				goto ChooseFilename;
			}
			if (!(fp1 = fopen("mid_cpp", "r")))
			{
				printf("无法打开中间文件！\n");
				printf("按任意键继续...");
				getchar(); getchar();
				goto ChooseFilename;
			}
			if (!program(fp1, T))
			{
				printf("程序存在语法错误！无法缩进打印！\n");
				printf("按任意键继续...");
				getchar(); getchar();
				goto ChooseFilename;
			}
			if (!(fp = freopen(filename, "r+", fp)))
			{
				printf("文件打开失败！\n");
				printf("按任意键继续...");
				getchar(); getchar();
				goto ChooseFilename;
			}
			Print_CFile(fp);
			printf("生成成功！请至源文件中查看。\n");
			printf("按任意键继续...\n");
			getchar(); getchar();
			break;
		case 4:
			goto ChooseFilename;
		default:
			return 0;
		}
	}
	return 1;
}