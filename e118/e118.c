/* Skriv ett program som tar bort alla inledande blanka tecken från en rad, och
   som raderar helt blanka rader. */

#include <stdio.h>

#define IN 1
#define OUT 0


main()
{
	int c, state = OUT;

	while((c = getchar()) != EOF) {
		if (state==OUT && c==' ')
			continue;
		else if (state==OUT && c=='\n')
			continue;
		else if (state==OUT)
			state = IN;
		else if (state==IN && c=='\n')
			state = OUT;
		putchar(c);
	}

	return 0;
}
