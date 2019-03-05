#include"Print_CFile.h"

Status Print_CFile(FILE * fp)
{
	FILE * fp1;
	int indent0, line = 1;
	int i; char c;
	if (!(fp1 = fopen("print_cpp", "w"))) return ERROR;
	while (!printList.empty())
	{
		indent0 = printList.front().indent;
		printList.pop();
		while (!printList.empty() && line == printList.front().num) printList.pop();
		//将起始行号相同的所有元素全部出队列，以免冲突
		while (!printList.empty() && line < printList.front().num)
		{
			if ((c = fgetc(fp)) == '#')
			{
				do
				{
					fputc(c, fp1);
				} while ((c = fgetc(fp)) != '\n');
				fputc(c, fp1);
			}
			else if (c == '\n');
			else
			{
				for (i = 0; i < indent0; i++)
					fputc('\t', fp1);
				if (c == '\t')
					while ((c = fgetc(fp)) == '\t');
				ungetc(c, fp);
				while ((c = fgetc(fp)) != '\n')
				{
					while (c == '\t') c = fgetc(fp);
					if (c == ' ') fputc(c, fp1);
					while (c == ' ') c = fgetc(fp);
					fputc(c, fp1);
				}
				fputc(c, fp1);
			}
			line++;
		}
	}
	for (i = 0; i < indent0; i++)
		fputc('\t', fp1);
	while ((c = fgetc(fp)) != EOF)
		fputc(c, fp1);
	//打印缩进为队列中最后一个元素存放值的行
	fclose(fp); fclose(fp1);
	return OK;
}