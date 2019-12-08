/* Gör ett program som jämför två filer och skriver ut första raden de skiljer sig
 * på.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer1[1024];
char buffer2[1024];

int main(int argc, char **argv)
{
	if(argc != 3) return 1;

	FILE *f1 = fopen(argv[1], "r");
	FILE *f2 = fopen(argv[2], "r");

	char *c1 = 0;
	char *c2 = 0;
	int n = 0;
	do {
		c1 = fgets(buffer1, 1024, f1);
		c2 = fgets(buffer2, 1024, f2);
		n++;
	} while (c1 && c2 && (strcmp(c1, c2) == 0));

	if (c1 || c2) {
		printf("Rad: %d\n",n);
		printf("Fil: %s\n", argv[1]);
		if (c1) printf("%s\n\n", c1);
		printf("Fil: %s\n", argv[2]);
		if (c2) printf("%s\n\n", c2);
	}
	return 0;
}
