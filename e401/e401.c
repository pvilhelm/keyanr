/* Skriv en funktion strrindex(s, t) som returerar den förekomst av t
 * i s som är längst till höger, eller -1 om det inte finns någon.
 */

#include <stdio.h>

/* Kanske borde gjort denna "baklänges" snarare än leta efter alla men
 * orka ..
 */
int strrindex(char s[], char t[])
{
	int i, j, k, idx = -1;

	for (i = 0; s[i] != '\0'; i++) {
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
			;
		if (k > 0 && t[k] == '\0')
			idx = i;
	}

	return idx;
}

int main(int argc, char **argv)
{
	for (int i = 1; i + 1 < argc; i += 2) {
		int idx = strrindex(argv[i], argv[i + 1]);
		printf("pos: %i sträng: \"%s\" sök: \"%s\" från idx: \"%s\"\n",
				idx, argv[i], argv[i + 1], idx >= 0 ? argv[i] + idx : 0);
	}

	return 0;
}
