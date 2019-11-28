/* Skriv en funktion expand(s1, s2) som expanderar förkortade uttryck som a-z
 * i sträng s1 till att vara den motsvarande fullständiga listan abc...xyz i
 * s2. Tillåt både gemener och versaler samt siffror och var beredd att hantera
 * fall typ a-b-c och a-z0-9 och -a-z. Se till att inledande eller avslutande "-"
 * tas bokstavligen.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NO_STATE 1
#define LETTER 2
#define SCORE 3

#define IN_RANGE(l, a, r) (((l) <= (a)) && ((a) >= (r)))


void expand(char *s1, char *s2)
{
	char c;
	char c_letter;

	int state = NO_STATE;

	while (c = *s1++) {
		switch (state) {
		case NO_STATE:
			if (isalnum(c)) {
				state = LETTER;
				c_letter = c;
				continue;
			} else {
				*s2++ = c;
				continue;
			}
			break;
		case LETTER:
			if (isalnum(c)) {
				*s2++ = c_letter;
				c_letter = c;
				continue;
			} else if (c == '-') {
				state = SCORE;
				continue;
			} else {
				*s2++ = c_letter;
				*s2++ = c;
				state = NO_STATE;
				continue;
			}
			break;
		case SCORE:
			if (isalnum(c)) {
				if (isalpha(c_letter) && isalpha(c)) {
					if (isupper(c_letter) && isupper(c) ||
							islower(c_letter) && islower(c)) {
						if (c_letter < c)
							for(int cc = c_letter; cc <= c; cc++)
								*s2++ = cc;
						else
							for(int cc = c_letter; cc >= c; cc--)
								*s2++ = cc;
					} else {
						*s2++ = c_letter;
						*s2++ = '-';
						*s2++ = c;
					}
				} else if (isdigit(c_letter) && isdigit(c)) {
					for(int cc = c_letter; cc <= c && cc <= '9'; cc++)
						*s2++ = cc;
				} else {
					/* Ogiltlig följd, tolka bokstavligt. */
					*s2++ = c_letter;
					*s2++ = '-';
					*s2++ = c;
				}
				state = NO_STATE;
				continue;
			} else {
				*s2++ = c_letter;
				*s2++ = '-';
				*s2++ = c;
				state = NO_STATE;
				continue;
			}
		}
	}
	if (state == LETTER || state == SCORE)
		*s2++ = c_letter;
	if (state == SCORE)
		*s2++ = '-';
	*s2++ = 0;
}


#define BUF_SIZE 1024

char out_buffer[BUF_SIZE];

main(int argc, char **argv)
{
	expand(argv[1], out_buffer);
	printf("%s", out_buffer);

	return 0;
}
