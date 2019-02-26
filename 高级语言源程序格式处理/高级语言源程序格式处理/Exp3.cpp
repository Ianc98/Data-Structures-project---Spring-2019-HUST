#include <stdio.h>
int a, b;
char _d, _e;
double c;
int func(int a, int b)
{
	int i;
	a = 1;
	b = 2;
	c = 3;
	_d = '\x22';
	_e = '\t';
	a = 1 + b;
	b = 2 + b;
	b = c;
	if (a == b)
		a = a + b;
	else if (a<b)
	{
		a = a - b;
		if (a>b) {
			a = a + 1;
		}
	}
	else {
		a = a / b;
		a = a + 1;
	}

	if (a>b)
		if (a>b)
			if (a>b)
				a++;

	if (a>b) {
		a = (a + 1) * 3;
		b = b + c + 2;
	}
	do
	{
		a++;
	} while (a>b);
	while (1) {
		if (a>b) {
			a = (a + 1) * 3;
			b = b + c + 2;
		}
		while (a == b) {
			a++;
		}
	}
	for (i = 0; i<a; i++) {
		a = a + 1;
	}
	for (i = 0;; i--) {
		a = (a + 1) * 2;
	}
	do {
		a = b + c;
	} while (a<100);
	return a + b;
}