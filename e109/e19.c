#include <stdio.h>

main()
{
	int c, in_space = 0;
	
	while ((c = getchar()) != EOF) {
		if (c == ' ')
			in_space = 1;
		else {
			if(in_space == 1)
				putchar(' '), putchar(c), in_space = 0;
			else
				putchar(c);
		}

	}

	return 0;
}
