/* Skriv getfloat, dvs. flyttalsversionen av getint. Vilken typ av funktionsvärde
 * returnerar getfloat?
 */

#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100
char buf[BUFSIZE]; 	/* buffer for ungetch */
int bufp = 0;		/* next free position in buf */

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: för många tecken\n");
	else
		buf[bufp++] = c;
}

int getfloat(float *pn)
{
	int c, sign;

	while (isspace(c=getch()))
		;
	if (!isdigit(c) && c != EOF && c != '+' && c != '-' && c != '.') {
		ungetch(c);
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-') {
		char cc = getch();
		if (!isdigit(cc) && cc != '.' || cc == EOF) {
			ungetch(cc);
			ungetch(c);
			return 0;
		}
		c = cc;
	}
	for (*pn = 0; isdigit(c); c = getch())
		*pn = 10 * *pn + (c - '0');
	*pn *= sign;
	if (c == '.') {
		int n;
		float dec;;
		for (dec = 0, n = 0; (c = getch()) && isdigit(c); n++)
				dec = 10 * dec + (c - '0');
		for (;n > 0;n--)
			dec /= 10.;
		*pn += sign * dec;
	}

	if (c != EOF)
		ungetch(c);
	return c;
}

int main( int argc, char **argv)
{
	char c;
	do {
		float f;
		c = getfloat(&f);
		if (c)
			printf("%f\n", f);
		else {
			bufp = 0;
		}
	} while (c != EOF);
}
