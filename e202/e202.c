
/* Skriv en for-slinga ekvivalent den ovan utan att använda || eller &&. */

#include <stdio.h>

main()
{
	/* for(i=0; i<lim-1 && (c=getchar())!= '\n' && c != EOF; ++i)
		s[i] = c; */
	char s[1024] = {0};
	int lim = 1024;
	int i;
	char c;

	for (i = 0; i < lim-1; i++) {
		if ((c = getchar()) != EOF) {
			if (c != '\n')
				s[i] = c;
			else
				break;
		} else
			break;
	}

	printf("%s", s);

	return 0;
}
