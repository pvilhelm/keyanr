/* "Skriv ett program som skriver ut alla rader längre än 80 tecken" */

#include <stdio.h>

#define MAXLINE 1000 /* Längsta inmatade rad. */

int getline2(char line[], int lim);
void copy(char to[], char from[]);

/* Skriv ut längsta inmatade rad. */
main()
{
	int len;		/* aktuell radlängd */
	char line[MAXLINE];	/* aktuell inmatad rad */

	while ((len = getline2(line, MAXLINE)) > 0)
		if (len > 80) {
			printf("%s", line);
		}
	return 0;
}

/* getline: läs in en rad i s, returnera längden */
int getline2(char s[], int lim)
{
	int c, i;

	for (i = 0; i<lim-1 && (c = getchar()) != EOF && c != '\n'; i++)
		s[i] = c;
	if (c == '\n') {
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	/* Läs in till radslutet om vi nådde max utan ett. */
	if (c != '\n' && c != EOF)
		while(((c = getchar()) != EOF) && c != '\n')
			i++;
	return i;
}

/* copy: kopiera 'from' in i 'to' */
/* förutsatt att to är tillräckligt stor */
void copy(char to[], char from[])
{
	int i;

	i = 0;
	while((to[i] = from[i]) != '\0')
		++i;
}
