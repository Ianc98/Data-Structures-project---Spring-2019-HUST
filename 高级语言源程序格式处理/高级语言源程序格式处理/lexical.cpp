#include "lexical.h"

char * token_text;

int gettoken(FILE * fp)
{
	token_text = NULL;
	while ((c = fgetc(fp)) == ' ');	//过滤掉空白符号，如果考虑报错位置，对回车需要单独
									//处理，每次回车，设置一个行数计数器加1
	if ((c >= 'A'&&c <= "Z") || (c >= 'a'&&c <= 'z'))
	{

	}
	return 0;
}
