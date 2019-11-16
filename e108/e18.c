#include <stdio.h>

main()
{

	int n = 0;
	int c;

	while ((c = getchar()) != EOF)
		if (c == ' ' || c == '\t' || c == '\n')
			n++;

	printf("\n%i antal mellanslag, tab och radmatningar", n);

	return 0;
}
