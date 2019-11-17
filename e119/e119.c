/* Skriv en funktion reverse(s) som kastar om tecken i strängen s. Använd
   den till ett program som skriver ut inmatade rader baklänges. */

#include <stdio.h>

#define BUF_SIZE 1000

char line_buffer[BUF_SIZE];

void reverse(char s[])
{
	int len;
	for (len = 0; len<BUF_SIZE && s[len] != '\0'; len++)
		;

	for (int i = 0; i < len/2; i++) {
		char tmp = s[i];
		s[i] = s[len - i - 1];
		s[len - i - 1] = tmp;
	}

	return;
}

main()
{
	int c, i = 0;

	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			line_buffer[i] = '\0';
			reverse(line_buffer);
			printf("%s\n", line_buffer);
			i = 0;
		} else if (i < BUF_SIZE - 1) {
			line_buffer[i++] = c;
		}
	}

	/* Skriv ut sista stympade raden om sådan finns. */
	if (i != 0) {
		line_buffer[i] = '\0';
		reverse(line_buffer);
		printf("%s", line_buffer);
	}

	return 0;
}
