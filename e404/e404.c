/* Givet ett grundläggande skelett är det enkelt att utvidga
 * kalkulatorn. Lägg till modulo-operatorn % och hantering av
 * negativa tal.
 */

/* Lägg till möjligheten att skriva ut toppen på stacken utan att
 * ta bort den, duplicera det och låta elementen byta plats.
 */

#include <stdio.h>
#include <stdlib.h> /* for atof() */
#include <math.h>

#define MAXOP 100	/* max size of operand or operator */
#define NUMBER 	'0' /* signal that a number was found */

double peek(void);
double swap(void);
double dup(void);

int getop(char []);
void push(double);
double pop(void);
int size(void);
/* reverse Polish calculator */
main()
{
	int type;
	double op2;
	char s[MAXOP];
	int tokens = 0;

	while ((type = getop(s)) != EOF) {

		if (type != '\n') tokens++;

		switch (type) {
			case NUMBER:
				push(atof(s));
				break;
			case '+':
				push(pop() + pop());
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '/':
				op2 = pop();
				if (op2 != 0.0)
					push(pop() / op2);
				else
					printf("fel: noll-nämnare\n");
				break;
			case '\n':
				if (tokens) {
					printf("\t\t%.8g\n", peek());
					tokens = 0;
				} else {
					if (size()) {
						printf("\t%.8g\n", peek());
						printf("\t\t%.8g\n", peek());
					}
					dup();
				}
				break;
			case '%':
				op2 = pop();
				if (op2 != 0.0)
					push(fmod(pop(), op2));
				else
					printf("fel: Modulo-operation med noll\n");
				break;
			case 's':
				swap();
				break;
			case 'd':
				dup();
				break;
			case 'p':
				printf("\t%.8g\n", peek());
				break;
			default:
				printf("fel: okänt uttryck %s\n", s);
			break;
		}
	}
	return 0;
}

#define MAXVAL 100 	/* maximum depth of val stack */
int sp = 0; 		/* next free stack position */
double val[MAXVAL]; /* value stack */



/* push: push f onto value stack */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("fel: stacken full, kan ej lägga till %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("fel: stacken tom\n");
		return 0.0;
	}
}

double peek(void)
{
	if (sp > 0)
		return val[sp - 1];
	else {
		printf("fel: tittade i en tom stack\n");
		return 0.0;
	}
}

double dup(void)
{
	if (sp > 0) {
		double top = peek();
		push(top);
		return top;
	} else {
		printf("fel: försökte duplicera toppen på en tom stack\n");
		return 0.0;
	}
}

double swap(void)
{
	if (sp > 1) {
		double top = pop();
		double sec = pop();
		push(top);
		push(sec);
		return sec;
	} else {
		printf("fel: försökte växla toppen på en för liten stack\n");
		return 0.0;
	}
}

int size(void)
{
	return sp;
}

#include <ctype.h>

int getch(void);
void ungetch(int);
/* getop: get next character or numeric operand */
int getop(char s[])
{
	int i, c, c2;

	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-' && c != '+')
		return c;
	/* not a number or minus plus */
	i = 0;

	c2 = getch();
	ungetch(c2);
	/* Se om det kommer ett tal efter '+-'-tecknet eller inte. */
	if ((c == '+' || c == '-') && !isdigit(c2) && c2 != '.') {
		/* Var en operator inte ett '+-'-tecken. */
		return c;
	}

	if (isdigit(c) || c == '+' || c == '-') /* collect integer part */
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.') /* collect fraction part */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

#define BUFSIZE 100
char buf[BUFSIZE]; 	/* buffer for ungetch */
int bufp = 0;		/* next free position in buf */

/* get a (possibly pushed-back) character */
int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

/* push character back on input */
void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: för många tecken\n");
	else
		buf[bufp++] = c;
}
