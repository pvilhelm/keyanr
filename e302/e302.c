/* Skirv en funktion escape(s, t) som omvandlar tecken som radmatning
 * och tabulator till synbara kontrollsekvenser som \n och \t när den kopierar sträng
 * t till s. Använd en switch-sats. Skriv också en funktion för andra hållet.
 */

#include <stdio.h>
#include <string.h>


char *escape(char *s, char *t, int t_size)
{
	if (!t) return 0;
	if (!s) return 0;

	char *t_end = t + t_size;
	char *t_start = t;

#define SPACE_LEFT_OR_RET(n) \
	do { if(t_end - t <= n) {*t_start = 0; return 0;}} while(0)

	char c;
	while(c = *s++) {
		switch(c){
		case '\t':
			SPACE_LEFT_OR_RET(2);
			*t++ = '\\';
			*t++ = 't';
			break;
		case '\n':
			SPACE_LEFT_OR_RET(2);
			*t++ = '\\';
			*t++ = 'n';
			break;
		case '\r':
			SPACE_LEFT_OR_RET(2);
			*t++ = '\\';
			*t++ = 'r';
			break;
		case '\a':
			SPACE_LEFT_OR_RET(2);
			*t++ = '\\';
			*t++ = 'a';
			break;
		case '\\':
			SPACE_LEFT_OR_RET(2);
			*t++ = '\\';
			*t++ = '\\';
			break;
		default:
			SPACE_LEFT_OR_RET(1);
			*t++ = c;
			break;
		}
		SPACE_LEFT_OR_RET(1);
	}
	*t = 0;

	return t_start;
#undef SPACE_LEFT_OR_RET
}

char *descape(char *s, char *t, int t_size)
{
	if (!t) return 0;
	if (!s) return 0;

	char *t_end = t + t_size;
	char *t_start = t;

	#define SPACE_LEFT_OR_RET(n) \
	do { if(t_end - t <= n) {*t_start = 0; return t_start;}} while(0)

	char c, cc;
	while(c = *s++) {
		if (c == '\\') {
			if (cc = *s++) {
				SPACE_LEFT_OR_RET(1);
				switch(cc){
				case 't':
					*t++ = '\t';
					break;
				case 'n':
					*t++ = '\n';
					break;
				case 'r':
					*t++ = '\r';
					break;
				case 'a':
					*t++ = '\a';
					break;
				case '\\':
					*t++ = '\\';
					break;
				default:
					*t++ = '\\';
					SPACE_LEFT_OR_RET(1);
					*t++ = c;
					break;
				}
			} else { /* Slut på strängen s. */
				SPACE_LEFT_OR_RET(2);
				*t++ = '\\';
				*t++ = 0;
			}
		} else {
			SPACE_LEFT_OR_RET(1);
			*t++ = c;
		}
		/* Se till att vi har plats skriva 0 i strängen t */
		SPACE_LEFT_OR_RET(1);
	}
	/* Terminera t */
	*t = 0;
	return t_start;

#undef SPACE_LEFT_OR_RET
}

main(int argc, char **argv)
{
	const int BUFFER_SIZE = 1024;

	char escaped[BUFFER_SIZE];
	char descaped[BUFFER_SIZE];

	{
		char test1[] = "Hej,\t\nJag heter Petter\a\a\a!";
		printf("%s\n", escape(test1, escaped, BUFFER_SIZE));

		descape(escaped, descaped, BUFFER_SIZE);
    	printf("descapde(espace()) = orginal? %i\n", strcmp(descaped, test1) == 0);
	}

	{
		char test1[] = "\t\\t";
		printf("%s\n", escape(test1, escaped, BUFFER_SIZE));
		descape(escaped, descaped, BUFFER_SIZE);
		printf("%s\n", descaped);
		printf("descapde(espace()) = orginal? %i\n", strcmp(descaped, test1) == 0);
	}

	return 0;
}
