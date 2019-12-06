/* Skriv versioner av biblioteksfunktionerna strncpy, strncat och strncmp
 * vilka operar på som mest de n första tecken i sina argumentsträngar.
 * T.ex. strncpy(s, t, n) kompierar som n tecken av t till s.
 * Fullständiga beskrivningar finns i appendix B.
 */

#include <stdio.h>
#include <stddef.h>

char *my_strncpy(char *s, char *t, size_t n)
{
	char *ss = s;
	size_t i;

	for (i = 0; i < n && *t != 0; i++)
		*ss++ = *t++;
	for (; i < n; i++)
		*ss++ = 0;
	return s;
}


char *my_strncat(char *s, char *t, size_t n)
{
	char *ss = s;
	/* Hitta slutet på s. */
	while(*ss != 0)
		ss++;

	for (int i = 0; i < n && *t != 0; i++)
		*ss++ = *t++;
	*ss = 0;

	return s;
}

int my_strncmp(char *s, char *t, size_t n)
{
	for (int i = 0; i < n; i++, s++, t++) {
		if (*s < *t) return -1;
		if (*s > *t) return 1;
		if (!*s) return 0;
	}
	return 0;
}

char test1A[100] = "abcde";
char test1B[] = "fgh";

char test2A[100] = "abcde";
char test2B[] = "fgh";

char test3A[100] = "abcde";
char test3B[] = "abc";
char test3C[] = "q";
char test3D[] = "abcdef";

int main(int argc, char **argv)
{
	my_strncpy(test1A, test1B, 2);
	printf("fgcde == %s\n", test1A);

	my_strncat(test2A, test2B, 2);
	printf("abcdefg == %s\n", test2A);

	printf("0 == %d\n", my_strncmp(test3A, test3B, 2));
	printf("0 == %d\n", my_strncmp(test3A, test3B, 3));
	printf("1 == %d\n", my_strncmp(test3A, test3B, 5));
	printf("0 == %d\n", my_strncmp(test3A, test3B, 0));

	int ans = my_strncmp(test3A, test3C, 1);
	printf("-1 == %d\n", ans);
	printf("-1 == %d\n", my_strncmp(test3A, test3C, 3));
	printf("0 == %d\n", my_strncmp(test3A, test3C, 0));

	printf("0 == %d\n", my_strncmp(test3A, test3D, 3));
	printf("-1 == %d\n", my_strncmp(test3A, test3D, 10));
	printf("-1 == %d\n", my_strncmp(test3A, test3D, 6));
	printf("0 == %d\n", my_strncmp(test3A, test3D, 5));

	return 0;
}
