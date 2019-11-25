/* Skriv en funktion setbits(x, p, n, y) som returnerar x med de n bitar som
   börjar vid position p satta till den n bitarna längst till höger i y, med
   de övriga bitarna lämnade oförändrade. */

#include <stdio.h>
#include <inttypes.h>

uint32_t setbits(uint32_t x, uint32_t p, uint32_t n, uint32_t y)
{
	uint32_t mask = (~0 << n << p) | ~(~0 << p);
	/* mask har nu ettor för de oberörda positionerna */

	uint32_t code = (y & ~(~0 << n)) << p;

	return (x & mask) | code ;
}

main()
{
	printf("Ska vara 2: ");
	printf("%x\n", setbits(0, 1,1,1));

	
	printf("Ska vara ffaf: ");
	printf("%x\n", setbits(0xffff, 4,4,0xa));

	printf("Ska vara 0xaaffacdf: ");
	printf("%x\n", setbits(0xaaffaaff, 4,8,0xabcd));

	printf("Ska vara 0x123cde78: ");
	printf("%x\n", setbits(0x12345678, 8,12,0xabcde));

	printf("Ska vara 0x12345678: ");
	printf("%x\n", setbits(0x12345678, 8, 0, 0xabcde));

	return 0;
}
