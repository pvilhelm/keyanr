/* Skriv en funktion itob(n, s,b) som omvandlar heltalet n till
 * att representeras med basen b i strängen s. itob(n, s, ) formaterar
 * till exempel n till ett hexadecimalt heltal i s.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void strrev(char *s)
{
	for (int j = 0, i = strlen(s) - 1; i > j; i--, j++){
		int c;
		c = s[j];
		s[j] = s[i];
		s[i] = c;
	}
}

void itob(int ni, int b, char s[])
{
	long long n = ni;
	int i, sign;

	if ((sign = n) < 0)
		n = -n;

	i = 0;

	do {
		int rem = n % b;
		if (rem >= 0 && rem <= 9)
			s[i++] = n % b + '0';
		else
			s[i++] = n % b + 'a' - 10;
	} while ((n /= b) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = 0;

	strrev(s);
}

char buffer[1024];

int main(int argc, char **argv)
{
	for(int i = 1; i  + 1 < argc; i += 2) {
		char *end_ptr;
		int b = atoi(argv[i + 1]);
		int n = strtol(argv[i], &end_ptr, b);
		itob(n, b, buffer);
		printf("arg: värde %3s bas %3s, val_10: %3i, itob: %3s\n",
				argv[i], argv[i + 1], n, buffer);
	}

	return EXIT_SUCCESS;
}
