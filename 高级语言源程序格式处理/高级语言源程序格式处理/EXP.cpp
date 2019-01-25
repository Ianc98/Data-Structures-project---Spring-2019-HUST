#include"lexical.h"
int main(void)
{
	int i, j;
	for (i = 0;i < 23;i++)
	{
		for (j = 0;j < 6;j++)
			printf("%s\t", HashKey[i][j]);
		printf("\n");
	}
	getchar();
	return 0;
}