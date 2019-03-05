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
		while (!printList.empty() && line < printList.front().num)
		{
			for (i = 0; i < indent0; i++)
				fputc('\t', fp1);
			while ((c = fgetc(fp)) != '\n')
				fputc(c, fp1);
			fputc(c, fp1);
			line++;
		}
	}
	for (i = 0; i < indent0; i++)
		fputc('\t', fp1);
	while ((c = fgetc(fp)) != EOF)
		fputc(c, fp1);
	fclose(fp); fclose(fp1);
	return OK;
}