/* Skriv funktionen strend(s, t), vilken återlämnar 1 om strängen t
 * förekommer sist i strängen s och noll annars.
 */

#include <stdio.h>
#include <string.h>

int strend(char *s, char *t)
{
	int len_s = strlen(s);
	int len_t = strlen(t);

	char *s_end = s + len_s;
	char *t_end = t + len_t;

	for (int i = 0;; i++) {
		if (i != len_s && i == len_t)
			return 1;
		else if (i == len_s && i != len_t)
			return 0;
		else if (i == len_s && i == len_t)
			return 1;
		else if (*--s_end != *--t_end)
			return 0;
	}

	return 1;
}


int main(int argc, char **argv)
{
	for (int i = 1; i + 1 < argc; i += 2)
	{
		printf("%i\n", strend(argv[i], argv[i + 1]));
	}

	return 0;
}
