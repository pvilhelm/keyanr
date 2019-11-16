#include <stdio.h>

#define IN 1
#define OUT 0

main()
{
	int c, state = 0;

	while((c = getchar()) != EOF) {
		if( c == ' ' || c == '\n' || c == '\t') {
			if(state == OUT)
				putchar(c);
			else {
				putchar('\n');
				putchar(c);
				state = OUT;
			}
		} else {
			state = IN;
			putchar(c);
		}
	}

	return 0;
}
