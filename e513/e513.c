/* Skriv programmet tail, vilket skriver ut de sista n raderna inmatat data.
 * Normalvärdet för n är, tex 10, men det kan ändras med ett valfritt argument, så att
 * 	tail -n
 * skriver ut de n sista raderna. Programmet skall uppföra sig förnufitgt
 * oavsett hur konstigt indatat ser ut. Skriv programmet så att det använder minnet
 * på bästa möjliga sätt. Raderna skall lagras som i sorteringspgrammet i avsnitt 5.6,
 * inte i en tvådimensionell vecktor med fast storlek.
 */

#include <stdlib.h>
#include <stdio.h>

#define MAX_ROWS 100
char *row_ptrs[MAX_ROWS];
char **sp_ptr = &row_ptrs[0];
int n_added;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char **argv)
{
	int n_rows;

	if (argc == 2) {
		char *end;
		n_rows = strtol(argv[1], &end, 0);
	} else
		n_rows = 10;

	char *c_ptr = 0;
	long unsigned int n = 0;
	int lines;
	while ((lines = getline(&c_ptr, &n, stdin)) > 0) {
		sp_ptr++;
		if (sp_ptr > row_ptrs + MAX_ROWS || sp_ptr - row_ptrs > n_rows) {
			sp_ptr = row_ptrs;
		}
		if (sp_ptr)
			free(*sp_ptr);
		*sp_ptr = c_ptr;
		n_added++;

		c_ptr = 0;
		n = 0;
	}


	for (int i = MIN(n_rows, n_added) - 1; i > 0; i--) {
		long long idx = sp_ptr - i - row_ptrs;
		if (idx < 0) {
			c_ptr = *(row_ptrs + n_rows + idx);
		} else
			c_ptr = *(row_ptrs + idx);
		printf("%s", c_ptr);
		free(c_ptr);
	}

	return 0;
}
