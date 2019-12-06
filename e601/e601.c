/* Vår version av getword behandlar inte understrykningstecken, strängkostanter,
 *  kommentarer eller preprocessor-kontrollrader på lämpligt sätt.
 *
 * Skriv en bättre version.
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct key {
    char *word;
    int count;
} keytab[] = {
    "auto", 0,
    "break", 0,
    "case", 0,
    "char", 0,
    "const", 0,
    "continue", 0,
    "default", 0,
    /* ... */
    "unsigned", 0,
    "void", 0,
    "volatile", 0,
    "while", 0
};

#define NKEYS (sizeof keytab / sizeof(struct key))


#define MAXWORD 100

int getword(char *, int);
int binsearch(char *, struct key *, int);

/* count C keywords */
main()
{
   int n;
   char word[MAXWORD];

   while (getword(word, MAXWORD) != EOF)
	   if (isalpha(word[0]))
		   if ((n = binsearch(word, keytab, NKEYS)) >= 0)
			   keytab[n].count++;
   for (n = 0; n < NKEYS; n++)
	   if (keytab[n].count > 0)
		   printf("%4d %s\n",
			   keytab[n].count, keytab[n].word);
   return 0;
}

/* binsearch:  find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n)
{
   int cond;
   int low, high, mid;

   low = 0;
   high = n - 1;
   while (low <= high) {
	   mid = (low+high) / 2;
	   if ((cond = strcmp(word, tab[mid].word)) < 0)
		   high = mid - 1;
	   else if (cond > 0)
		   low = mid + 1;
	   else
		   return mid;
   }
   return -1;
}

/* getword:  get next word or character from input */
int getword(char *word, int lim)
{
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    while (isspace(c = getch()))
        ;
    if (c != EOF)
        *w++ = c;

    /* Ta bort kommentarer */
	if (c == '/') {
		char cc = getch();
		if (cc == '*') {
			*w = 0;
			while((c = getch()) != EOF) {
				if (c == '*') {
					cc = getch();
					if (cc == '/') {
						/* Hittade slutet på kommentaren. */
						return cc;
					} else if (cc == EOF) return EOF;
				}
				if (c == EOF) return EOF;
			}
			if (c == EOF) return EOF;
		} else
			ungetch('*');
	}

	/* Ta bort strängar */
	if (c == '"') {
		int is_escaped = 0;
		while((c = getch()) != EOF) {
			if (!is_escaped && c == '"')
				return c;
			else if (is_escaped)
				is_escaped = 0;
			else if (c == '\\')
				is_escaped = 1;
		}
		if (c == EOF) return EOF;
	}
	/* Ta bort char literals */
	if (c == '\'') {
		int is_escaped = 0;
		while((c = getch()) != EOF) {
			if (!is_escaped && c == '\'')
				return c;
			else if (is_escaped)
				is_escaped = 0;
			else if (c == '\\')
				is_escaped = 1;
		}
		if (c == EOF) return EOF;
	}

    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }


    for ( ; --lim > 0; w++)
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

#define BUFSIZE 100

char buf[BUFSIZE];    /* buffer for ungetch */
int bufp = 0;         /* next free position in buf */

int getch(void)  /* get a (possibly pushed-back) character */
{
   return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)   /* push character back on input */
{
   if (bufp >= BUFSIZE)
	   printf("ungetch: too many characters\n");
   else
	   buf[bufp++] = c;
}
