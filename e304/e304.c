/* Med två komplementrepresentation, klarar itoa inte av det största
 * negativa talet, dvs när n har värdet -(2^(ordstorlek-1)). Förklara varför
 * detta inte går. Ändra funktionen så den hanterar värdet korrekt
 * oberoende av vilken maskin den körs på.
 */

 /* Eftersom tecknet "tas bort" så blir -2^31 0 istället för 2^31. */
#define BUG 0

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

void itoa(int ni, char s[])
{
#if BUG != 0
	int n = ni;
#else
	long long n = ni;
#endif
	int i, sign;

	if ((sign = n) < 0)
		n = -n;

	i = 0;

	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = 0;

	strrev(s);
}

char buffer[1024];

int main(int argc, char **argv)
{
	for(int i = 1; i < argc; i++) {
		int n = atoi(argv[i]);
		itoa(n, buffer);
		printf("itoa: %s\n", buffer);
	}

	return EXIT_SUCCESS;
}
