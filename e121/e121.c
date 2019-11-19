/* Skriv ett program entab som ersätter strängar av mellanslag med ett minimtalt
   antal tabulatortecken och mellanslag för att uppnå samma effekt. Avnänd samma
   tabulatorstopp som för detab. När antingen ett tabulatortecken eller ett 
   mellanslag räcker för att nå ett tabulatorstopp, vilket gör ges företräde? */

#include <stdio.h>

#define TAB_SIZE 8


void print_n_spaces(int n)
{
	for(int i = 0; i < n; i++) putchar(' ');
}

main()
{
	int c;
	int n_chars = 0;
	int n_spaces = 0;
	int n_tabspaces = 0;

	while((c = getchar()) != EOF) {
		if (c == '\t') {
			/* Hur många mellanslag motsvarar en tab nu? */
			int rem = TAB_SIZE - (n_tabspaces + n_chars + n_spaces) % TAB_SIZE;
			n_tabspaces += rem;
		} else if (c == ' ') {
			n_spaces++;
		} else { /* Inte tab, mellanslag eller ny rad .. */
			/* Hur många mellanslag motsvarar en tab efter sista icke whitespace? */
			int n_spaces_first_tab = TAB_SIZE - n_chars % TAB_SIZE;

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
				int rem_tabs = (n_total_spaces - n_spaces_first_tab) / TAB_SIZE;
				for(int i = 0; i < rem_tabs; i++) putchar('\t');
				n_chars += rem_tabs;

				/* Fyll ut med mellanslag. */
				int rem_spaces = n_total_spaces - n_spaces_first_tab
							- TAB_SIZE * rem_tabs;
				print_n_spaces(rem_spaces);
				n_chars += rem_spaces;

			}
			n_spaces = 0;
			n_tabspaces = 0;
			n_chars++;
			if (c == '\n') n_chars = 0;
			putchar(c);
		}
	}
	return 0;
}
