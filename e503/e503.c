/* Skriv en pekarversion av funktionen  strcat som vi visade i kapitel 2.
 * strcat(s, t) kopierar strängen t till slutet av s.
 */

void my_strcat(char *s, char *t)
{

	while (*s != '\0')
		s++;
	while ((*s++ = *t++) != '\0')
		;
}

#include <stdio.h>

main()
{
	char c[100] = "abcd";
	char d[] = "efg";

	my_strcat(c, d);
	printf("%s", c);
}

