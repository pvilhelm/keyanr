/* Skriv om lämpligt program från tidigare kapitel och övningar med
 * pekare istället för vektor-indexeringar.
 *
 */

#include <stdio.h>

int getline2(char *s,int lim)
{
	char *s_start = s;

	int c;
	while ((s - s_start) < lim-1 && (c=getchar())!=EOF && c!='\n')
		*s++ = c;
	if (c == '\n')
		*s++ = c;
	*s++ = '\0';

	return s - s_start;
}

char buffer[1024];

main()
{
	while (getline2(buffer, 16) > 1)
		printf("%s", buffer);

	return 0;
}
