/* Skriv ett program som tar bort alla kommentarer från ett C-program. Glöm
   inte att hantera strängar inom cituationstecken eller teckenkonstanter rätt.
   C-kommentarer kan inte nästas. */

#include <stdio.h>
#include <stdlib.h>

#define SLASH 8
#define STAR 16
#define IN_COMMENT 1
#define ESCAPED 2
#define IN_QUOTE 4
#define NO_STATE 0
#define IN_CHARLIT 32
#define IN_CHARLITEND 64

typedef struct {int row; int col;} pos_t;

char line_buffer[1024];

void print_line_buffer(int row, int col)
{
	fprintf(stderr, "%d:%d:\t%s\n", row, col, line_buffer);
}

main()
{
	int state = NO_STATE;
	int c;
	int col = 0;
	int row = 1;
	
	int paran_depth = 0;
	int hookparan_depth = 0;
	int birdparan_depth = 0;

	pos_t start_quote = {0};
	pos_t start_comment = {0};
	pos_t start_paran = {0};
	pos_t start_hookparan = {0};
	pos_t start_birdparan = {0};

	while ((c = getchar()) != EOF) {
start:
		/*fprintf(stderr, "%c %8x rad %d kol %d\n", c, state, row, col); */

		if (c == '\n')
			row++, col = 0;
		else if (c == '\t')
			col += 4;
		else
			col++;
	
		if (c != '\n' && col < 1024 - 1) {
			line_buffer[col - 1] = c;
			line_buffer[col] = 0;
		}

		if (state == NO_STATE) {

			if (c == '/')
				state = SLASH;
			else if (c == '"') {
				state = IN_QUOTE;
				start_quote.row = row;
				start_quote.col = col;
			} else if (c == '\'')
				state = IN_CHARLIT;
			else if (c == '(') {
				paran_depth++;
				if (paran_depth == 1) {
					start_paran.row = row;
					start_paran.col = col;
				}
			} else if (c == '[') {
				hookparan_depth++;
				if (hookparan_depth == 1) {
					start_hookparan.row = row;
					start_hookparan.col = col;
				}
			} else if (c == '{') {
				birdparan_depth++;
				if (birdparan_depth == 1) {
					start_birdparan.row = row;
					start_birdparan.col = col;
				}
			} else if (c == ')') {
				if(--paran_depth < 0) {
					fprintf(stderr, "Obalnserad parantes. För många ')'.\n");
					print_line_buffer(row, col);
					exit(1);
				}
			} else if (c == ']') {
				if(--hookparan_depth < 0) {
					fprintf(stderr, "Obalnserad hakparantes. För många ']'.\n");
					print_line_buffer(row, col);
					exit(1);
				}
			} else if (c == '}') {
				if(--birdparan_depth < 0) {
					fprintf(stderr, "Obalnserade måsvingar. För många '}'.\n");
					print_line_buffer(row, col);
					exit(1);
				}
			}

			continue;

		} else if (state == SLASH) {
			if (c == '*') {
				state = IN_COMMENT;
				start_comment.col = col;
				start_comment.row = row;
				continue;
			} else {
				state = NO_STATE;
				goto start; /* Låt inläst char behandlas igen */
			}
		} else if (state == IN_COMMENT) {
			if (c == '*')
				state |= STAR;
			else if (c == '/')
				state |= SLASH;
			continue;
		} else if (state == (IN_COMMENT | STAR)) {
			if (c == '/') /* Inte lägre i en kommentar. */
				state = NO_STATE;
			else
				state = IN_COMMENT;
			continue;
		} else if (state == (IN_COMMENT | SLASH)) {
			/* '/''*' får inte finnas i kommentarer. */
			if (c == '*') {
				fprintf(stderr, "Kommentar börjar i kommentar.\n");
				print_line_buffer(row, col);
				exit(1);
			}
			state = IN_COMMENT;
			continue;
		} else if (state == IN_QUOTE) {
			if (c == '\\') {
				state |= ESCAPED;
			} else if (c == '"')
				state = NO_STATE;
			continue;
		} else if (state == (IN_QUOTE | ESCAPED)) {
				state = IN_QUOTE;
		} else if (state == IN_CHARLIT) {
			if (c == '\\')
				state |= ESCAPED;
			else if (c=='\'') {
				fprintf(stderr, "Char literal med '' i följd.\n");
				print_line_buffer(row, col);
				exit(1);
			} else
				state = IN_CHARLITEND;
		} else if (state == (IN_CHARLIT | ESCAPED)) {
			state = IN_CHARLITEND;
		} else if (state == IN_CHARLITEND) {
			if (c != '\'') {
				fprintf(stderr, "Char literal har för många tecken.\n");
				print_line_buffer(row, col);
				exit(1);	
			} else {
				state = NO_STATE;
			}
		} else {
			fprintf(stderr, "Bugg!");
			exit(1);
		}

	}


	if (birdparan_depth) {
		fprintf(stderr, "Felaktigt balanserade måsvingar till slutet av filen.\n"
"Startposition: rad %d kol %d\n", start_birdparan.row, start_birdparan.col);
		exit(1);
	}
	if (paran_depth) {
		fprintf(stderr, "Felaktigt balanserade paranteser till slutet av filen.\n"
"Startposition: rad %d kol %d\n", start_paran.row, start_paran.col);
		exit(1);
	}
	if (hookparan_depth) {
		fprintf(stderr, "Felaktigt balanserade hakparanteser till slutet av filen.\n"
"Startposition: rad %d kol %d\n", start_hookparan.row, start_hookparan.col);
		exit(1);
	}
	if (IN_QUOTE & state) {
		fprintf(stderr, "Felaktigt balanserade citattecken till slutet av filen.\n"
"Startposition: rad %d kol %d\n", start_quote.row, start_quote.col);
		exit(1);
	}
	if (IN_COMMENT & state) {
		fprintf(stderr, "Felaktigt balanserade kommentarer till slutet av filen.\n"
"Startposition: rad %d kol %d\n", start_comment.row, start_comment.col);
		exit(1);
	}
	if ((IN_CHARLIT & state) || (IN_CHARLITEND & state)) {
		fprintf(stderr, "Felaktigt balanserade \"'\" till slutet av filen.\n"
"Startposition: rad %d kol %d\n", 0, 0);
		exit(1);
	}

	return 0;
}
