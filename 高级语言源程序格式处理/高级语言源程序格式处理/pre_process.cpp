#include"pre_process.h"
using namespace std;
FILE * pre_process(FILE * fp)
{
	FILE * fp_mid;	//fp_mid用来存放中间文件的指针
	char c;
	char string[100];
	char * container;
	char filename[] = "mid_cpp";	//中间代码的文件名
	queue<char *> search;
	queue<char *> object;
	int length;		//存放字符串长度
	int flag = 0;		//用于记录找到宏定义的状态
	if (!(fp_mid = fopen(filename, "w"))) return NULL;
	if (!feof(fp))
	{
		c = fgetc(fp);
		if (c == '#')
		{
			fscanf(fp, "%s", string);
			if (!strcmp(string, "define"))
			{
				container = (char *)malloc(100 * sizeof(char));
				fscanf(fp, "%s", container);
				search.push(container);
				container = (char *)malloc(100 * sizeof(char));
				fscanf(fp, "%s", container);
				object.push(container);
			}
		}
	}
	search.push(NULL); object.push(NULL);
	while (!feof(fp))
	{
		c = fgetc(fp);
		if (c == '\n')
		{
			c = fgetc(fp);
			if (c == '#')
			{
				fscanf(fp, "%s", string);
				if (!strcmp(string, "define"))
				{
					container = (char *)malloc(100 * sizeof(char));
					fscanf(fp, "%s", container);
					search.push(container);
					container = (char *)malloc(100 * sizeof(char));
					fscanf(fp, "%s", container);
					object.push(container);
				}
			}
		}
	}
	fseek(fp, 0, SEEK_SET);
	if (!feof(fp))
	{
		c = fgetc(fp);
		if (c == '#')
		{
			fscanf(fp, "%s", string);
			if (!strcmp(string, "define"))
			{
				fscanf(fp, "%s", string);
				if ((c = fgetc(fp)) != ' ') return NULL;
				fscanf(fp, "%s", string);
			}
			else if (!strncmp(string, "include", 7))
			{
				if (!string[7])
				{
					c = fgetc(fp); c = fgetc(fp);
					if (c != '<'&&c != '"') return NULL;
					else if (c == '<')
					{
						fscanf(fp, "%s", string);
						if (string[strlen(string) - 1] != '>') return NULL;
					}
					else
					{
						fscanf(fp, "%s", string);
						if (string[strlen(string) - 1] != '"') return NULL;
					}
				}
				else
				{
					if (string[7] != '<'&&string[7] != '"') return NULL;
					else if (string[7] == '<')
						if (string[strlen(string) - 1] != '>') return NULL;
						else
							if (string[strlen(string) - 1] != '"') return NULL;
				}
			}
			else return NULL;
		}
		else ungetc(c, fp);
	}
	while (!feof(fp))
	{
		while ((c = fgetc(fp)) == '\n')
		{
			fputc(c, fp_mid);
			c = fgetc(fp);
			if (c == '#')
			{
				fscanf(fp, "%s", string);
				if (!strcmp(string, "define"))
				{
					fscanf(fp, "%s", string);
					if ((c = fgetc(fp)) != ' ') return NULL;
					fscanf(fp, "%s", string);
				}
				else if (!strncmp(string, "include", 7))
				{
					if (!string[7])
					{
						c = fgetc(fp); c = fgetc(fp);
						if (c != '<'&&c != '"') return NULL;
						else if (c == '<')
						{
							fscanf(fp, "%s", string);
							if (string[strlen(string) - 1] != '>') return NULL;
						}
						else
						{
							fscanf(fp, "%s", string);
							if (string[strlen(string) - 1] != '"') return NULL;
						}
					}
					else
					{
						if (string[7] != '<'&&string[7] != '"') return NULL;
						else if (string[7] == '<')
						{
							if (string[strlen(string) - 1] != '>') return NULL;
						}
						else
							if (string[strlen(string) - 1] != '"') return NULL;
					}
				}
				else return NULL;
			}
			else ungetc(c, fp);
		}
		while (container = search.front())
		{
			if (c == container[0])
			{
				ungetc(c, fp);
				fgets(string, strlen(container) + 1, fp);
				if (!strcmp(string, container))
				{
					fputs(object.front(), fp_mid);
					flag = 1;
					while (container = search.front())
					{
						search.pop();
						search.push(container);
						container = object.front();
						object.pop();
						object.push(container);
					}
				}
				else
				{
					length = strlen(container);
					fseek(fp, -length, SEEK_CUR);
					c = fgetc(fp);
				}
			}
			else
			{
				search.pop();
				search.push(container);
				container = object.front();
				object.pop();
				object.push(container);
			}
		}
		search.pop();
		search.push(container);
		container = object.front();
		object.pop();
		object.push(container);
		if (!flag) fputc(c, fp_mid);
		flag = 0;
	}
	fclose(fp_mid); fclose(fp);
	return fp_mid;
}
