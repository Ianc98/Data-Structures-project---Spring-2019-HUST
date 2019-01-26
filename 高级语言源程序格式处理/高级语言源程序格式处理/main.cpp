#include"lexical.h"
int main(void)
{
	char filename[30]; FILE * fp;
	int status;
	printf("输入文件名：");
	scanf("%s", &filename);
	if (!(fp = fopen(filename, "r")))
	{
		printf("文件打开失败！\n");
		getchar(); return 0;
	}
	printf("	单词类别		单词值\n");
	while (!feof(fp))
	{
		status = gettoken(fp);
		if (status >= AUTO && status <= _THREAD_LOCAL)
			printf("	关键字");
	}
	return 1;
}