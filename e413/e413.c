/* Skriv en rekursiv version av funktionen reverse(s), som kastar om ordningen
 * i en sträng utan att skapa en ny.
 */

#include <stdio.h>
#include <string.h>

void reverse(char *s, char *e)
{
	if (!e)
		e = s + strlen(s);
	if (e - s <= 1)
		return;

	char c = *(e - 1);
	*(e - 1) = *s;
	*s = c;

	reverse(s + 1, e - 1);

	return;
}


char buffer[1024];

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		printf("%s ->", argv[i]);
		reverse(argv[i], 0);
		printf(" %s", argv[i]);
	}
}
