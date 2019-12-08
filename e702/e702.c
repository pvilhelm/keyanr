/* Skriv ett program som skriver ut godtycklig inmatning på ett förnuftigt sätt. Som
 * ett minimum bör det skriva icke-grafiska tecken oktalt eller hexadecimalt enligt
 * lokal praxis, och bryta långa textrader.
 */


#include <stdio.h>
#include <ctype.h>

int main(int argc, char **argv)
{
	char c;
	int n = 0;
	while ((c = getchar()) != EOF) {
		n++;
		if(n > 60) {putchar('\n');n = 1;}

		if (isspace(c)) {putchar(c); continue;}
		if (isprint(c)) {putchar(c); continue;}
		if (c == '\a') {printf("\\a"); continue;}
		if (c == '\r') {printf("\\r"); continue;}
		if (c == '\b') {printf("\\b"); continue;}
		if (c == '\f') {printf("\\f"); continue;}

		printf("{0x%x}", (unsigned char)c);
	}
	printf("\\EOF");

	return 0;
}
