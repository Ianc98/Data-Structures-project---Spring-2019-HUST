#include"Print_CFile.h"

Status Print_CFile(FILE * fp, char filename[30])
{
	FILE *fp_std; int indent = 0;	//Ëõ½øÁ¿
	char c; int i = 0; int flag = 1;
	char filename_std[33];
	strcpy(filename_std, filename);
	strcat(filename_std, "_std");
	if (!(fp_std = fopen(filename_std, "w"))) return ERROR;
	while (!feof(fp))
	{
		while (flag)
		{
			c = fgetc(fp);
			switch (c)
			{
			case '{':
				printf("{");
				indent++; flag = 1;
				break;
			case ' ':
				printf(" ");
				flag = 1;
				break;
			case '\n':
				printf("\n");
				if ((c = fgetc(fp)) == '}')
					indent--;
				ungetc(c, fp);
				flag = 1;
				for (i = 0; i < indent; i++)
					printf("\t");
				break;
			default:
				ungetc(c, fp);
				flag = 0;
				break;
			}
		}
		i = gettoken(fp);
		if (i == IF || i == WHILE || i == ELSE)
		{
			printf("%s", token_text);
			while ((c = fgetc(fp)) != '\n' && c != ';'&&c!='{')
				printf("%c", c);
			if (c == '\n')
			{
				printf("%c", c);
				if ((c = fgetc(fp)) != '{')
				{
					indent++;
					for (i = 0; i < indent; i++) putchar('\t');
					while (c != ';')
					{
						printf("%c", c);
						c = fgetc(fp);
					}
					printf("%c", c);
					indent--;
				}
			}
			else ungetc(c, fp);
		}
		else printf(token_text);
		flag = 1;
	}
	fclose(fp_std);
	return OK;
}