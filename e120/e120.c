/* Skriv ett program detab som ersätter tabulatortecken i inmatningen med rätt
   antal mellanslag för att komma till nästa tabulatorstopp. Antag ett konstant
   antal tabulatorstopp, till exempel för varje n kolumner. Bör n vara en variabel
   eller en symbolisk konstant? */

#include <stdio.h>

#define TAB_SIZE 8


void print_n_spaces(int n)
{
	for(int i = 0; i < n; i++) putchar(' ');
}

/* Ersätt \t med så många mellanslag att nästa tecken hamnar på TAB_SIZE:te
   kolumnen. */
main()
{
	int c, n_chars = 0;

	while((c = getchar()) != EOF) {
		if (c == '\n') {
			n_chars = 0;
			putchar(c);
		} else if (c != '\t') {
			n_chars++;
			putchar(c);
		} else { /* '\t' */
			int rem = TAB_SIZE - n_chars % TAB_SIZE;
			print_n_spaces(rem);
			n_chars += rem;
		}
	}

	return 0;
}
