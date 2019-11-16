#include <stdio.h>

#define IN 1
#define OUT 0

#define MAX_WORD_SIZE 64

int word_sizes[MAX_WORD_SIZE];

main()
{
	int c, state, n;

	state = OUT;
	n = 0;

	while((c = getchar()) != EOF) {
		if( c == ' ' || c == '\n' || c == '\t') {
			if(state == IN) {
				state = OUT;
				if (n < MAX_WORD_SIZE) word_sizes[n]++;
				else word_sizes[MAX_WORD_SIZE - 1]++;
				n = 0;
			}
		} else {
			state = IN;
			n++;
		}
	}
	/* Ta hand om sista ordet. */
	if (state == IN) {
		if (n < MAX_WORD_SIZE) word_sizes[n]++;
		else word_sizes[MAX_WORD_SIZE - 1]++;
	}

	/* Skriv ut ett lodrätt histogram. */

	/* Börja med att hitta hur högt det ska vara. */
	int max = 1;
	for (int i = 0; i < MAX_WORD_SIZE; i++)
		if (word_sizes[i] > max) max = word_sizes[i];

	printf("Skriver ut histogram över antal bokstäver per ord.\n\n");
	/* Rita ut en # för varje rad som har ett värde som
  	   är lika med eller understiger nuvarande höjd.  	*/
	for (int i = 0; i < max; i++) {
		printf("         %3i - ", max - i);
		for (int j = 0; j < MAX_WORD_SIZE; j++) {
			if (word_sizes[j] >= max - i) putchar('#');
			else putchar(' ');
		}
		putchar('\n');
	}

	/* Gör en fin x-axel */
	printf("                1   ");
	for (int i = 5; i < MAX_WORD_SIZE; i++)
		if (i%5 == 0) printf("%-5i", i);

	printf("\nAntal bostäver->^\n");
	char yaxis[] = "|Antal ord";
	for(int i = 0; i < sizeof(yaxis); i++) {
		for(int j = 0; j < 16; j++) putchar(' ');
		putchar(yaxis[i]);
		putchar('\n');
	}

	return 0;
}
