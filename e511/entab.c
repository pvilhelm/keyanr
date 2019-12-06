/* Skriv ett program entab som ersätter strängar av mellanslag med ett minimtalt
   antal tabulatortecken och mellanslag för att uppnå samma effekt. Avnänd samma
   tabulatorstopp som för detab. När antingen ett tabulatortecken eller ett
   mellanslag räcker för att nå ett tabulatorstopp, vilket gör ges företräde? */

#include <stdio.h>
#include <stdlib.h>


void print_n_spaces(int n)
{
	for(int i = 0; i < n; i++) putchar(' ');
}

int main(int argc, char **argv)
{
	int c;
	int n_chars = 0;
	int n_spaces = 0;
	int n_tabspaces = 0;

	int tab_size = 8;
	int offset = 0;

	for (int i = 1; i < argc; i++) {
		switch(*argv[i]) {
		case '+':
			tab_size = atoi(argv[i] + 1);
			break;
		case '-':
			offset = atoi(argv[i] + 1);
			break;
		}
	}

	while((c = getchar()) != EOF) {
		if (c == '\t') {
			/* Hur många mellanslag motsvarar en tab nu? */
			int rem = tab_size - (n_tabspaces + n_chars + n_spaces) % tab_size;
			n_tabspaces += rem;
		} else if (c == ' ') {
			n_spaces++;
		} else { /* Inte tab, mellanslag eller ny rad .. */
			/* Hur många mellanslag motsvarar en tab efter sista icke whitespace? */
			int n_spaces_first_tab = tab_size - n_chars % tab_size;

			int n_total_spaces = n_spaces + n_tabspaces;
			/* Vi specialbehandlar fallet att en tab nu vore
			   för många motsvarande mellanslag. */
			if (n_spaces_first_tab > n_total_spaces) {
				print_n_spaces(n_total_spaces);
				n_chars += n_total_spaces;
			} else {
				/* Den första ska alltid vare en tab när den får
				   plats. */
				putchar('\t');
				n_chars++;

				/* Hur många tabs till behövs? */
				int rem_tabs = (n_total_spaces - n_spaces_first_tab) / tab_size;
				for(int i = 0; i < rem_tabs; i++) putchar('\t');
				n_chars += rem_tabs;

				/* Fyll ut med mellanslag. */
				int rem_spaces = n_total_spaces - n_spaces_first_tab
							- tab_size * rem_tabs;
				print_n_spaces(rem_spaces);
				n_chars += rem_spaces;

			}
			n_spaces = 0;
			n_tabspaces = 0;
			n_chars++;
			if (c == '\n') n_chars = offset;
			putchar(c);
		}
	}
	return 0;
}
