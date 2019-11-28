/* I ett 2-komplements talsystem tar x &= (x-1) bort 1-biten längst till
 * höger i x. Förklara varför. Använd denna observation för att skriva
 * en snabbare version av bitcount.
 */

/* x &= (x - 1) kommer att få den högraste kombitinationen av 100..00 att
 * gå till 011..11 och när den sedan och:as försvinner alla ettorna inkl.
 * x högraste (som ju och:as med 0).
 */

#include <stdio.h>

int bitcount(unsigned x)
{
	int b = 0;

	for (; x; b++)
		x &= (x - 1);

	return b;
}

main()
{
	printf("0b10101010 => 4 : %i\n", bitcount(0b10101010));
	printf("0b11101010 => 5 : %i\n", bitcount(0b11101010));
	return 0;
}
