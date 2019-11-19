/* Skriv ett program för att bryta långa inmatade rader till två eller flera kortare rader
   efter det sista icke-blanka tecknet som förekommer före den n-te kolumnen. Se till att
   programmet gör något intelligent med mycket långa rader, och när det inte finns några
   mellanslag eller tabulatortecken före den specifierade kolumnen. */

#include <stdio.h>
#include <assert.h>

#define MAX_COL 10

char line_buffer[MAX_COL];

void print_line_buffer(int idx_to)
{
	assert(idx_to < MAX_COL);
	int n;

	if (idx_to < 0) n = MAX_COL;
	else n = idx_to + 1;

	for(int i = 0; i < n; i++)
		putchar(line_buffer[i]);
}

main()
{
	int c = 0;
	int idx_buffer = 0; /*Första tomma platsen i buffern. */


	while ((c = getchar()) != EOF) {
		if (c != '\n' && idx_buffer < MAX_COL) {
			line_buffer[idx_buffer++] = c;
		} else if (c == '\n' && idx_buffer <= MAX_COL) {
			for(int i = 0; i < idx_buffer; i++)
				putchar(line_buffer[i]);
			putchar('\n');
			idx_buffer = 0;
		} else {
			/* Hitta sista icke-mellanslaget */
			int idx_last_char = -1;
			for(int i = idx_buffer - 1; i >= 0; i--) {
				if(line_buffer[i] != ' ') {
					idx_last_char = i;
					break;
				}
			}

			if (idx_last_char == -1) {
				print_line_buffer(-1);/* Skriver ut hela buffern. */
				putchar('\n');
				line_buffer[0] = c;
				idx_buffer = 1;
			} else {
				print_line_buffer(idx_last_char);
				putchar('\n');
				/* Flytta eventuella rester i buffern till början. */
				int step = idx_last_char + 1;
				for (int i = step; i < MAX_COL; i++)
					line_buffer[i - step] = line_buffer[i];
				idx_buffer = MAX_COL - step;
				line_buffer[idx_buffer++] = c;
			}

		}
	}
	print_line_buffer(idx_buffer - 1);

	return 0;
}
