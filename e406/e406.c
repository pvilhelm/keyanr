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

/* Lägg till kommandon för att hantera variabler. (Det är lätt att
 * tillhandahplla tjugosex variabler med en-bokstaviga namn.).
 * Lägg till en variabel för det senast utskrivna värdet.
 */

#include <stdio.h>
#include <stdlib.h> /* for atof() */
#include <math.h>
#include <string.h>
#include <ctype.h>

#define VAR_NAME_MAXSIZE 12

#define MAXOP 100	/* max size of operand or operator */
#define NUMBER 	'0' /* signal that a number was found */

#define BUFSIZE 100
#define LINE_BUFFER_SIZE 1024

#define MAXVAL 100 	/* maximum depth of val stack */

/* Typer och enumer */

enum stack_type {
	NO_TYPE,
	DOUBLE,
	VAR,
};

typedef struct {
	enum stack_type type;
	double dval;
	char name[VAR_NAME_MAXSIZE];
} stack_node_t;

typedef struct {
	double dval;
	char name[VAR_NAME_MAXSIZE];
} var_t;

typedef struct {
	char *str;
	int n_args;
	double (*fnc_ptr)(void);
	char *help;
} fnc_dispath_t;

double ans;

char buf[BUFSIZE]; 	/* buffer for ungetch */
int bufp = 0;		/* next free position in buf */

char line_buffer[LINE_BUFFER_SIZE];

int sp = 0; 		/* next free stack position */
stack_node_t val[MAXVAL]; /* value stack */

#define MAXVAR 32
var_t vars[MAXVAR];
int n_vars = 0;

void push(double);
void push_node(stack_node_t node);
void push_var(double f, char *s);
stack_node_t pop_node(void);
double pop(void);
int size(void);

double get_var_value(char *name);
var_t *find_var(char *name);
void define_var(char *name, double value);
void set_var_value(char *name, double dval);

int char_in_str(char c, char *s); /* Returnerar 1 om c är i s, annars 0. */
int is_number_literal(char *tkn); /* Returnerar 1 om tkn är ett tal, annars 0. */
/* Läser in och returerar pekare till rad. eof sätts
 * till 1 om EOF påträffas.
 */
char *get_line(int *eof);
/* Letar efter passande operand eller funktion för ett token
 * samt exekverar det. */
void process_token(char *tkn);

double peek(void); /* Returnerar toppen av stacken, eller noll om topp saknas. */
double swap(void); /* Byter toppen av stacken med underliggade, eller ingenting om de saknas. */
double dup(void); /* Kopierar toppen av stacken och lägger på stacken. */

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

double fnc_less(void)
{
	double op2 = pop();
	push(pop() < op2);
	return 0.;
}

double fnc_more(void)
{
	double op2 = pop();
	push(pop() > op2);
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

double fnc_isequal(void)
{
	double op2 = pop();
	double op1 = pop();
	push(op2 == op1);
	return 0.;
}


double fnc_assign(void)
{
	stack_node_t op2 = pop_node();
	stack_node_t op1 = pop_node();

	if (op2.type == VAR) {
		if (op1.type == DOUBLE) {
			if (!find_var(op2.name))
				define_var(op2.name, op1.dval);
			else {
				set_var_value(op2.name, op1.dval);
			}
			push_node(op1);
		} else if(op1.type == VAR) {
			if (!find_var(op1.name)) {
				printf("fnc_assign: Variabeln %s inte definierad. \n", op1.name);
				return 0.;
			}
			if (!find_var(op2.name))
				define_var(op2.name, get_var_value(op1.name));
			else {
				set_var_value(op2.name, get_var_value(op1.name));
			}
			push_node(op1);
			return get_var_value(op1.name);
		}
	} else
		printf("fnc_assign: Kan inte tilldela. Andra operanden inte en variabel. \n");

	return 0.;
}

double fnc_ans(void)
{
	push(ans);

	return 0.;
}



double fnc_stack(void)
{

	for (int i = 0; i < sp - 1; i++)
		printf("%2d:\t\t%.8g\n", sp - i, val[i].dval);

	return 0.;
}

fnc_dispath_t fnc_arr[];

double fnc_help(void)
{
	printf("Detta är en stack-kalkulator. De allra flesta operatorer och funktioner lägger"
			" sitt svar på stacken. Variablers värden lagras separat, men referenser kan läggas på stacken.\n");
	for (int i = 0; fnc_arr[i].fnc_ptr != 0; i++) {
		printf("%s :    %s\n", fnc_arr[i].str, fnc_arr[i].help);
	}

	return 0.;
}

fnc_dispath_t fnc_arr[] =

{
		{"+", 2, add, "Summera de två översta operanderna på stacken.\n"},
		{"/", 2, divide, "Dela den näst översta operanden med den översta.\n"},
		{"%", 2, mod, "Räkna ut resten efter att den näst översta operanden delats med den översta."},
		{"-", 2, sub, "Den översta operanden minus den näst översta.\n"},
		{"*", 2, mul, "Gångre de två översta operanderna.\n"},
		{"<", 2, fnc_less, "Returnerar 1 om den näst översta operanden är mindre än den översta, annars 0.\n"},
		{">", 2, fnc_more, "Returnerar 1 om den näst översta operanden är mindre än den översta, annars 0.\n"},
		{"dup",0, dup,"Duplicera den översta operanden.\n"},
		{"swap",0, swap, "Byt plats på de två översta operanderna.\n"},
		{"peek",0, peek, "Värdet på den översta operanden.\n"},
		{"max",2, max, "Det största av de två översta operanderna.\n"},
		{"min",2, min, "Det minsta av de två översta operanderna.\n"},
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
		{":=", 2, fnc_assign, "Definerar en variabel med värdet av den"
				" översta operanden på stacken. Påverkar ej stacken.\n"},
		{"=", 2, fnc_isequal, "Returnerar 1 om de två översta operanderna har samma värde, annars 0.\n"},
		{"help", 1, fnc_help, "Skriver ut en hjältext.\n"},
		{"ans", 0, fnc_ans, "Motsvarar det senaste resultat som skrevs ut på stdout.\n"},
		{"stack",0, fnc_stack, "Skriver ut stackens värden.\n"},
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
		ans = peek();
		if (!eof && line && *line) /* Skriv inte ut tomma rader som innehåller EOF. */
			printf("\t\t%.8g\n", ans);
	} while(!eof);

	return 0;
}


/* push: push f onto value stack */
void push(double f)
{
	stack_node_t node = {};
	node.dval = f;
	node.type = DOUBLE;
	if (sp < MAXVAL)
		val[sp++] = node;
	else
		printf("fel: stacken full, kan ej lägga till %g\n", f);
}

void push_node(stack_node_t node)
{
	if (sp < MAXVAL)
		val[sp++] = node;
	else
		printf("push_node: stacken full, kan ej lägga till node\n");
}

/* pop: pop and return top value from stack */
double pop(void)
{
	if (sp > 0) {
		stack_node_t node = val[--sp];
		if (node.type == DOUBLE) {
			val[sp] = (stack_node_t){};
			return node.dval;
		} else if (node.type == VAR) {
			val[sp] = (stack_node_t){};
			return get_var_value(node.name);
		}
		printf("pos: bugg\n");
		return 0.0;
	} else {
		printf("pos: stacken tom\n");
		return 0.0;
	}
}

stack_node_t pop_node(void)
{
	if (sp > 0) {
		stack_node_t node = val[--sp];
		val[sp] = (stack_node_t) {};
		return node;
	} else {
		printf("pop_node: stacken tom\n");
		return (stack_node_t){};
	}
}

double peek(void)
{
	if (sp > 0) {
		stack_node_t node = val[sp - 1];
		if (node.type == DOUBLE) {
			val[sp] = (stack_node_t){};
			return node.dval;
		} else if (node.type == VAR) {
			val[sp] = (stack_node_t){};
			return get_var_value(node.name);
		}
		printf("peek: bugg\n");
		return 0.0;
	} else {
		printf("peek: tittade i en tom stack\n");
		return 0.0;
	}
}

double dup(void)
{
	double d;
	if (sp > 0) {
		stack_node_t node = pop_node();
		push_node(node);
		push_node(node);
		if (node.type == DOUBLE) {
			val[sp] = (stack_node_t){};
			d = node.dval;
		} else if (node.type == VAR) {
			val[sp] = (stack_node_t){};
			if (find_var(node.name))
				d = get_var_value(node.name);
			else
				d = 0.;
		}

		return d;

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
	HEXDIGITS,
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
			} else if (c == 'x' || c == 'X') {
				state = HEXDIGITS;
				continue;
			}
			return 0; /* Inte ett nummer. */
		case HEXDIGITS:
			if (isdigit(c) || char_in_str(tolower(c), "abcdef")) {
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

		return;
	}
	 /* Kolla om det är en operand */
	{
		fnc_dispath_t *ptr = 0;
		for (ptr = fnc_arr; ptr->str != 0; ptr++)
		{
			if (!strcmp(ptr->str, tkn)) {
				break;
			}
		}

		if (ptr->fnc_ptr) {
			ptr->fnc_ptr();
			return;
		}
	}

	/* Är det en variabel? */
	{
		int is_var = isalpha(*tkn);
		for (char *ptr = tkn; *ptr != 0 && is_var; ptr++)
			if (!isalnum(*ptr) && *ptr != '_') is_var = 0;
		if (is_var) {
			push_var(0., tkn);
			return;
		}
	}

	printf("process_token: token \"%s\" kan inte hanteras.\n", tkn);

	return;
}

void push_var(double f, char *s)
{
	stack_node_t node = {};
	node.dval = f;
	node.type = VAR;
	my_strncpy(node.name, s, VAR_NAME_MAXSIZE - 1);

	/*var_t var;
	var.dval = f;
	strncpy(var.name, s, VAR_NAME_MAXSIZE - 1);*/

	if (sp >= MAXVAL) {
		printf("fel: stacken full, kan ej lägga till %g\n", f);
		return;
	}
	/*
	if (n_vars >= MAXVAR) {
		printf("fel: För stort antal variabler full, kan ej lägga till %s\n", s);
		return;
	}
	*/

	val[sp++] = node;
	/*vars[n_vars++] = var;*/
}

double get_var_value(char *name)
{
	var_t *var = find_var(name);
	if (var) {
		return var->dval;
	}

	printf("get_var_value: Variablen %s är inte definierad.\n", name);
	return 0.;
}

void set_var_value(char *name, double dval)
{
	var_t *var = find_var(name);
	if (!var) {
		printf("set_var_value: Variablen %s är inte definierad.\n", name);
		return;
	}

	var->dval = dval;
}

var_t *find_var(char *name)
{
	var_t var;
	for (int i = 0; i < n_vars; i++) {
		if (strcmp(name, vars[i].name) == 0) {
			return &vars[i];
		}
	}
	return 0;
}

void define_var(char *name, double value)
{
	var_t var;
	my_strncpy(var.name, name, VAR_NAME_MAXSIZE);
	var.dval = value;
	if (n_vars < MAXVAR)
		vars[n_vars++] = var;
	else
		printf("define_var: För många variabler redan definerade.\n");

	return;
}
