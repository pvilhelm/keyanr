/* Första tecknet i s1 som motsvarar ett i s2 */

#include <stdio.h>

#define BUFFER_SIZE 1024

int any(char s1[], char s2[])
{
	int i, k;

	for (i = 0; s1[i] != '\0'; i++) {
		for (k = 0; s1[i] != s2[k] && s2[k] != '\0'; k++)
			;
		if (s2[k]) /* Tecken matchade de vi söker efter */
			return i;
	}

	return -1;
}


main()
{
	char line_buffer[BUFFER_SIZE];
	char ban_buffer[BUFFER_SIZE];

	char c;
	int i = 0;
	int arg_cntr = 0;
	printf("Ange sträng att söka i\n");
	while((c = getchar()) != EOF) {
		if (c == '\n' && i == 0)
			continue; /* filtrera bort dubbel-enter */

		if ((arg_cntr == 0 && c == '\n') || i == BUFFER_SIZE - 1) {
			arg_cntr++;
			line_buffer[i] = 0;
			i = 0;
			printf("Ange sträng att hitta tecken ur\n");
		} else if ((arg_cntr == 1 && c == '\n') || i == BUFFER_SIZE - 1) {
			arg_cntr++;
			ban_buffer[i] = 0;
			i = 0;
		} else if (arg_cntr == 0)
			line_buffer[i++] = c;
		else if (arg_cntr == 1)
			ban_buffer[i++] = c;

		if (arg_cntr == 2) {
			int pos = any(line_buffer, ban_buffer);
			printf("Tecken pos: %d\n", pos);
			arg_cntr = 0;
			printf("\nAnge sträng att söka i\n");
		}

	}

	return 0;
}
