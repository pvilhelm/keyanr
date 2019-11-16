
#include <stdio.h>

main()
{
	int c;
	while (1) {
		c = getchar();
		putchar(c);
		if (c != EOF == 0) {
			printf("\nc != EOF är 0!\n");
			break;
		}
		else if (c != EOF == 1)
			printf("\nc != EOF är 1!\n");
		else
			printf("\nc != EOF är någonting annat!\n");
	}
     		
	return 0;
}
