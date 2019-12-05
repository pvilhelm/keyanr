/* Givet ett grundläggande skelett är det enkelt att utvidga
 * kalkulatorn. Lägg till modulo-operatorn % och hantering av
 * negativa tal.
 */

/* Lägg till möjligheten att skriva ut toppen på stacken utan att
 * ta bort den, duplicera det och låta elementen byta plats.
 */

/* Lägg till möjligheten att använda biblioteksfunktioner som
 * sin, exp och pow. Titta i math.h i appendix B, avsnitt 4.
 */

#include <stdio.h>
#include <stdlib.h> /* for atof() */
#include <math.h>
#include <string.h>

#define MAXOP 100	/* max size of operand or operator */
#define NUMBER 	'0' /* signal that a number was found */

double peek(void); /* Returnerar toppen av stacken, eller noll om topp saknas. */
double swap(void); /* Byter toppen av stacken med underliggade, eller ingenting om de saknas. */
double dup(void); /* Kopierar toppen av stacken och lägger på stacken. */

int getop(char []);
void push(double);
double pop(void);
int size(void);

int char_in_str(char c, char *s); /* Returnerar 1 om c är i s, annars 0. */
int is_number_literal(char *tkn); /* Returnerar 1 om tkn är ett tal, annars 0. */
/* Läser in och returerar pekare till rad. eof sätts
 * till 1 om EOF påträffas.
 */
char *get_line(int *eof);
/* Letar efter passande operand eller funktion för ett token
 * samt exekverar det. */
void process_token(char *tkn);

/* Lägg ihop op 1 och 2 och lägg på stacken. */
double add(void)
{
	push(pop() + pop());

	return 0.;
}

/* Subtrahera operand 2 från 1 och lägg på stacken. */
double sub(void)
{
	double op2 = pop();
	push(pop() - op2);

	return 0.;
}

double mul(void)
{
	push(pop() * pop());

	return 0.;
}

double divide(void)
{
	double op2 = pop();
	if (op2 != 0.0)
		push(pop() / op2);
	else
		printf("fel: noll-nämnare\n");

	return 0.;
}

double mod(void)
{
	double op2 = pop();
	if (op2 != 0.0)
		push(fmod(pop(), op2));
	else
		printf("fel: Modulo-operation med noll\n");

	return 0.;
}

double max(void)
{
	double op2 = pop();
	double op1 = pop();
	push(op2 > op1 ? op2: op1);
	return 0.;
}

double fnc_sin(void)
{
	double op1 = pop();
	push(sin(op1));

	return 0.;
}

double fnc_cos(void)
{
	double op1 = pop();
	push(cos(op1));

	return 0.;
}

double fnc_tan(void)
{
	double op1 = pop();
	push(tan(op1));

	return 0.;
}

double fnc_asin(void)
{
	double op1 = pop();
	push(asin(op1));

	return 0.;
}

double fnc_acos(void)
{
	double op1 = pop();
	push(acos(op1));

	return 0.;
}

double fnc_atan(void)
{
	double op1 = pop();
	push(atan(op1));

	return 0.;
}

double fnc_sind(void)
{
	double op1 = pop();
	push(sin(op1*M_PI*2./360.));

	return 0.;
}

double fnc_cosd(void)
{
	double op1 = pop();
	push(cos(op1*M_PI*2./360.));

	return 0.;
}

double fnc_tand(void)
{
	double op1 = pop();
	push(tan(op1*M_PI*2./360.));

	return 0.;
}

double fnc_asind(void)
{
	double op1 = pop();
	push(asin(op1)*(1/M_PI/2.*360.));

	return 0.;
}

double fnc_acosd(void)
{
	double op1 = pop();
	push(acos(op1)*(1/M_PI/2.*360.));

	return 0.;
}

double fnc_atand(void)
{
	double op1 = pop();
	push(atan(op1)*(1/M_PI/2.*360.));

	return 0.;
}

double fnc_pow(void)
{
	double op1 = pop();
	double op2 = pop();
	push(pow(op1, op2));

	return 0.;
}

double fnc_exp(void)
{
	double op1 = pop();
	double op2 = pop();
	push(pow(op1, op2));

	return 0.;
}

double fnc_sqrt(void)
{
	double op1 = pop();
	push(sqrt(op1));

	return 0.;
}


double min(void)
{
	double op2 = pop();
	double op1 = pop();
	push(op2 < op1 ? op2: op1);
	return 0.;
}


typedef struct {
	char * str;
	int n_args;
	double (*fnc_ptr)(void);
} fnc_dispath_t;

fnc_dispath_t fnc_arr[] =

{
		{"+", 2, add},
		{"/", 2, divide},
		{"%", 2, mod},
		{"-", 2, sub},
		{"*", 2, mul},
		{"dup",0, dup},
		{"swap",0, swap},
		{"peek",0, peek},
		{"max",2, max},
		{"min",2, min},
		{"sin",2, fnc_sin},
		{"cos",2, fnc_cos},
		{"tan",2, fnc_tan},
		{"asin",2, fnc_asin},
		{"acos",2, fnc_acos},
		{"atan",2, fnc_atan},
		{"sind",2, fnc_sind},
		{"cosd",2, fnc_cosd},
		{"tand",2, fnc_tand},
		{"asind",2, fnc_asind},
		{"acosd",2, fnc_acosd},
		{"atand",2, fnc_atand},
		{"pow",2, fnc_pow},
		{"exp",1, fnc_exp},
		{"sqrt",1, fnc_sqrt},
		{0,0,0},
};

/* reverse Polish calculator */
main()
{
	int eof = 0;

	do {
		/* Läs in en rad från stdin. */
		char *line = get_line(&eof);
		/* Dela upp raden i tokens avskilda av \0 map. " ". */
		char *token = strtok(line, " ");
		/* Prcessa tokenet. */
		while (token) {
			process_token(token);
			token = strtok(0, " ");
		}
		printf("top:\t\t%.8g\n", peek());
	} while(!eof);

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
		printf("pos: stacken tom\n");
		return 0.0;
	}
}

double peek(void)
{
	if (sp > 0)
		return val[sp - 1];
	else {
		printf("peek: tittade i en tom stack\n");
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

#define LINE_BUFFER_SIZE 1024
char line_buffer[LINE_BUFFER_SIZE];

char *get_line(int *eof)
{
	char c;
	int i = 0;
	while((c = getchar()) != EOF)
	{
		if (c == '\n') {
			line_buffer[i++] = 0;
			return line_buffer;
		}

		if (c == '\t')
			line_buffer[i++] = ' ';
		else
			line_buffer[i++] = c;

		if (i == LINE_BUFFER_SIZE) {
			printf("get_line: för många tecken");
			return 0;
		}
	}

	line_buffer[i++] = 0;
	*eof = 1;

	return line_buffer;
}

enum number_state {
	NOT_NUMBER,
	PLUSMINUS,
	PREDOT_DIGITS,
	DOT,
	POSTDOT_DIGITS,
	E,
	ESIGN,
	EDIGITS,
};

int char_in_str(char c, char *s)
{
	char cc;

	while((cc = *s++) != 0)
		if (cc == c) return 1;

	return 0;
}

int is_number_literal(char *tkn)
{
	char c;
	enum number_state state = NOT_NUMBER;
	int is_number = 0;

	while((c = *tkn++) != 0) {
		switch(state) {
		case NOT_NUMBER:
			if (char_in_str(c, "+-")) {
				state = PLUSMINUS;
				continue;
			} else if (isdigit(c)) {
				state = PREDOT_DIGITS;
				continue;
			} else if (c == '.') {
				state = DOT;
				continue;
			}
			return 0; /* Inte ett nummer. */
		case PLUSMINUS:
			if (isdigit(c)) {
				state = PREDOT_DIGITS;
				continue;
			}
			return 0; /* Inte ett nummer. */
		case PREDOT_DIGITS:
			if (isdigit(c)) {
				continue;
			} else if (c == '.') {
				state = DOT;
				continue;
			} else if (c == 'e' || c == 'E') {
				state = E;
				continue;
			}
			return 0; /* Inte ett nummer. */
		case DOT:
			if (isdigit(c)) {
				state = POSTDOT_DIGITS;
				continue;
			} else if (c == 'E' || c == 'e') {
				state = E;
				continue;
			}
			return 0; /* Inte ett nummer. */
		case POSTDOT_DIGITS:
			if (isdigit(c)) {
				continue;
			} else if (c == 'E' || c == 'e') {
				state = E;
				continue;
			}
			return 0; /* Inte ett nummer. */
		case E:
			if (isdigit(c)) {
				state = EDIGITS;
				continue;
			} else if (char_in_str(c, "+-")) {
				state = ESIGN;
				continue;
			}
			return 0; /* Inte ett nummer. */
		case EDIGITS:
			if (isdigit(c)) {
				continue;
			}
			return 0; /* Inte ett nummer. */
		case ESIGN:
			if (isdigit(c)) {
				state = EDIGITS;
				continue;
			}
			return 0; /* Inte ett nummer. */
		}
	}

	if (state == PLUSMINUS ||
			state == NOT_NUMBER ||
			state == E)
		return 0;

	return 1;
}

void process_token(char *tkn)
{
	/* Kolla först om tokenet kan vara en talkonstant. */
	if (is_number_literal(tkn)) {
		char *end = 0;
		double ans = strtod(tkn, &end);

		if (end != tkn)
			push(ans);
		else {
			long lans = strtol(tkn, &end, 0);
			if (end != tkn)
				push((double)lans);
			else
				printf("Ogitligt tal: %s\n", tkn);
		}
	} else { /* Kolla om det är en operand */
		fnc_dispath_t *ptr = 0;
		for (ptr = fnc_arr; ptr->str != 0; ptr++)
		{
			if (!strcmp(ptr->str, tkn)) {
				break;
			}
		}

		if (ptr->fnc_ptr)
			ptr->fnc_ptr();
		else
			printf("process_token: token \"%s\" inte en funktion.\n", tkn);
	}

	return;
}
