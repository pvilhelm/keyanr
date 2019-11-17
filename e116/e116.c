#include <stdio.h>

#define MAXLINE 1000 /* Längsta inmatade rad. */

int getline2(char line[], int lim);
void copy(char to[], char from[]);

/* Skriv ut längsta inmatade rad. */
main()
{
	int len;		/* aktuell radlängd */
	int max;		/* maxlängd hittills */
	char line[MAXLINE];	/* aktuell inmatad rad */
	char longest[MAXLINE]; 	/* längsta rad sparas här */

	max = 0;
	while ((len = getline2(line, MAXLINE)) > 0)
		if (len > max) {
			max = len;
			copy(longest, line);
		}
	if (max > 0) {
		printf("%s", longest);
		printf("\nLängd: %i", max);
		if(max > MAXLINE) printf("\nSkriver bara ut %d första tecknen", MAXLINE);
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
