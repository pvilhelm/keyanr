#include <stdio.h>

main()
{
	int c, in_space = 0;
	
	while ((c = getchar()) != EOF) {
		if (c == '\b')
			printf("\\b");
		else if (c == '\\')
			printf("\\\\");
		else if (c == '\t')
			printf("\\t");
		else
			putchar(c);
	}

	return 0;
}
