/* Skriv om funktionen lower, som omvandlar versaler till gemener,
 * med ett villkorsuttryck istället för if-else.
 */

#include <stdio.h>

char lower(char c)
{
	return c <= 'Z' && c >= 'A' ? c - 'A' +'a' : c;
}

main()
{
	printf("%c lower -> %c\n",'A',lower('A'));
	printf("%c lower -> %c\n",'B',lower('b'));
	printf("%c lower -> %c\n",'g',lower('g'));
	printf("%i lower -> %i\n",'\a',lower('\a'));
	return 0;
}
