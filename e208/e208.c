/* Skriv en funktion rightrot(x, n) som returnerar värdet av heltalet x
 * högerroterat med n bitpositioner.
 */

#include <stdio.h>

unsigned rightrot(unsigned x, unsigned n)
{
	unsigned rc = x ^ (~0 < n);
	unsigned code = x >> n;

	/* Lägg till det som roterades ut åt höger. */
	return code | (rc << (32 - n));
}

main()
{
	printf("rightrot(0x0ff0, 4) == 0x00ff? bool==%i, värde==%x\n",
			rightrot(0x0ff0, 4) == 0x00ff, rightrot(0x0ff0, 4));

	printf("rightrot(0x1234, 4) == 0x40000123? bool==%i, värde==%x\n",
			rightrot(0x1234, 4) == 0x40000123, rightrot(0x1234, 4));

	printf("rightrot(1, 3) == 1 << 29? bool==%i, värde==%x\n",
				rightrot(1, 3) == 1 << 29, rightrot(1, 3));

	printf("rightrot(0x1234, 0) == 0x1234? bool==%i, värde==%x\n",
				rightrot(0x1234, 0) == 0x1234, rightrot(0x1234, 0));

	return 0;

}
