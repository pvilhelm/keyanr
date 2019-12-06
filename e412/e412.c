/* Utnyttja ideerna i printd för att skriva en rekursiv version av itoa, dvs
 * konvertera ett heltal till en sträng genom att anropa en rekursiv rutin.
 */

#include <stdio.h>
#include <stdlib.h>

int itoa(int n, char s[])
{
	int i, sign;
	int d = 1;
	long long nn = n;

	if (nn < 0) {
		nn = -nn;
		*s++ = '-';
	}

	if (nn/10) {
		d += itoa(nn/10, s);
		*(s + d - 1) = nn % 10 + '0';
		*(s + d) = 0;
	} else {
		*s++ = nn % 10 + '0';
		*s = 0;
	}

	return d;
}

char buffer[1024];

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		int n = atoi(argv[i]);
		itoa(n, buffer);
		printf("%i -> %s\n", n, buffer);
	}
}
