/* Skriv en alternativ version squeeze(s1, s2), som tar bort alla tecken i s1 som 
   matchar något tecken i s2. */

#include <stdio.h>

#define BUFFER_SIZE 1024

void squeeze(char s1[], char s2[])
{
	int i, j, k;

	for (i = j = 0; s1[i] != '\0'; i++) {
		for (k = 0; s1[i] != s2[k] && s2[k] != '\0'; k++)
			;
		if (!s2[k]) /* Inget tecken matchade de förbjudna */
			s1[j++] = s1[i];
	}
	s1[j] = '\0';

	return;
}


main()
{
	char line_buffer[BUFFER_SIZE];
	char ban_buffer[BUFFER_SIZE];

	char c;
	int i = 0;
	int arg_cntr = 0;
	printf("Ange sträng att filtrera\n");
	while((c = getchar()) != EOF) {
		if ((arg_cntr == 0 && c == '\n') || i == BUFFER_SIZE - 1) {
			arg_cntr++;
			line_buffer[i] = 0;
			i = 0;
			printf("Ange sträng att banna tecken ur\n");
		} else if ((arg_cntr == 1 && c == '\n') || i == BUFFER_SIZE - 1) {
			arg_cntr++;
			ban_buffer[i] = 0;
			i = 0;
		} else if (arg_cntr == 0)
			line_buffer[i++] = c;
		else if (arg_cntr == 1)
			ban_buffer[i++] = c;

		if (arg_cntr == 2) {
			squeeze(line_buffer, ban_buffer);
			printf("Filterat:\n%s\n", line_buffer);
			arg_cntr = 0;
			printf("\nAnge sträng att filtrera\n");
		}

	}

	return 0;
}
