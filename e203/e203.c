/* Skriv en funktion htoi(s), som omvandlar en sträng av hexadecimala siffror
   (inkluderande en valgri 0x eller 0X till sitt motsvarande heltaksvärde. De
   tillåtna siffrorna är 0 till 9, a till f och A till F. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

unsigned int char_to_uint(char c) 
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	fprintf(stderr, "Felaktigt tecken %c ... avbryter", c);
	exit(1);
}

unsigned int htoi(char s[])
{
	unsigned int number = 0;

	int len = strlen(s);

	if (len >= 3 && (s[1] == 'x' || s[1] == 'X')) {
		s = s + 2;
		len -= 2;
	}

	for (int i = 0; i < len; i++) {
		unsigned int n = char_to_uint(s[i]);
		for (int j = 1; j < len - i; j++)
			n *= 16;
		number += n;
	}

	return number;
}

main()
{
	char c;
	char buffer[BUFFER_SIZE];
	int i = 0;

	while ((c = getchar()) != EOF) {
		if (c == '\n' || c == ' ' || c == '\t') {
			if(!i) continue;
			buffer[i] = 0;
			i = 0;
			printf("%u\n",htoi(buffer));
		} else {
			if (i == BUFFER_SIZE - 1) continue;
			buffer[i++] = c;
		}
	}

	return 0;
}
