/* Som getint är skriven behandlas ett + eller -, som inte följs av en siffra, som
 * en gitlig representation av noll. Korrigera så att ett sådant tecken skickas
 * tillbaka till strömmen av indata.
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

int getint(int *pn)
{
	int c, sign;

	while (isspace(c=getch()))
		;
	if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetch(c);
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-') {
		char cc = getch();
		if (!isdigit(cc) || cc == EOF) {
			ungetch(cc);
			ungetch(c);
			return 0;
		}
		c = cc;
	}
	for (*pn = 0; isdigit(c); c = getch())
		*pn = 10 * *pn + (c - '0');
	*pn *= sign;
	if (c != EOF)
		ungetch(c);
	return c;
}

int main( int argc, char **argv)
{
	char c;
	do {
		int i;
		c = getint(&i);
		if (c)
			printf("%i\n", i);
		else {
			bufp = 0;
		}
	} while (c != EOF);
}

