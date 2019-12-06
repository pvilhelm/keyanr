/* Skriv programmet expr vilket evaluerar ett uttryck från kommandoraden
 * som är skrivet på formen Polsk notation (postfix), där varje operator e
 * eller operand är ett separat argument.
 *
 * Tex. expr 2 3 4 + *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024

double stack[STACK_SIZE];
double *stack_ptr = stack; /* Pekar på första lediga posten i stacken. */
const double *stack_end = stack + STACK_SIZE;

double pop(void)
{
	if (stack_ptr > stack)
		return *--stack_ptr;
	else
		printf("pop: Bugg\n");
	return 0.;
}

void push(double d)
{
	if (stack_ptr < stack_end) {
		*stack_ptr++ = d;
		return;
	}
	printf("push: Bugg\n");
	return;
}

void process_token(char *c)
{
	double op1, op2;
	if (strcmp(c, "+") == 0) {
		op2 = pop();
		op1 = pop();
		push(op1 + op2);
		return;
	} else if(strcmp(c, "-") == 0){
		op2 = pop();
		op1 = pop();
		push(op1 - op2);
		return;
	} else if(strcmp(c, "*") == 0){
		op2 = pop();
		op1 = pop();
		push(op1 * op2);
		return;
	} else if(strcmp(c, "/") == 0){
		op2 = pop();
		op1 = pop();
		push(op1 / op2);
		return;
	} else {
		/* Antagligen en siffra. */
		char *end;
		op1 = strtof(c, &end);
		if (c == end) {
			printf("process_token: Felaktigt tal %s\n", c);
		} else {
			push(op1);
		}
		return;
	}
}

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
		process_token(argv[i]);
	double ans = pop();
	printf("\t\t%g\n", ans);
}
