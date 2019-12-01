/* Skriv en version av itoa som accepterar tre argument istället för två.
 * Det tredje argumentet är en minimal fältbredd. Det omvandlade talet
 * måste fyllas ut med blanka tecken till vänster, så att hela fältet
 * blir fyllt.
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

void itoa(int ni, char s[], int f)
{
	const int b = 10;
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
	for(; i < f; i++)
		s[i] = ' ';
	s[i] = 0;

	strrev(s);
}

char buffer[1024];

int main(int argc, char **argv)
{
	for(int i = 1; i  + 1 < argc; i += 2) {
		char *end_ptr;
		int f = atoi(argv[i + 1]);
		int n = atoi(argv[i]);
		itoa(n, buffer, f);
		printf("arg: värde %3s fältbredd %3s, val_10: %3i, itoa: |%s|\n",
				argv[i], argv[i + 1], n, buffer);
	}

	return EXIT_SUCCESS;
}
