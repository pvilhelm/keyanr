/* Skriv ett program som beroende på med vilket namn (återfinns i argv[0]) det
 * anropas som, omvandlar gemener till versaler eller tvärt om.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int to_lower = 0;

	printf("%s\n", argv[0]);

	if (strcmp(argv[0], "./gemen") == 0) {
		to_lower = 1;
	}
	size_t n = 0;
	int nn = 0;
	char *c = 0;
	while ((nn = getline(&c, &n, stdin)) > 0) {
		for (char *cc = c; cc && *cc != 0; cc++)
			if (to_lower)
				putchar(tolower(*cc));
			else
				putchar(toupper(*cc));
		free(c);
	}

	return 0;
}
