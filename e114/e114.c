#include <stdio.h>

#define MAX_WORD_SIZE 256

int n_chars[MAX_WORD_SIZE];

main()
{
	int c, state, n;

	while((c = getchar()) != EOF) {
		if(c < 256) n_chars[c]++;
	}

	for(int i = 0; i < MAX_WORD_SIZE; i++) {
		if (n_chars[i] > 0) {
			if ((i <= 'z' && i >= 'a') || (i <= 'Z' && i >= 'A'))
				printf("%c   ", i);
			else
				printf("%-3i ", i);

			for(int j = 0; j < n_chars[i]; j++) 
				putchar('#');
			putchar('\n');
		}
	}

	return 0;
}
