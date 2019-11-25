/* Skriv en funktion invert(x, p, n), som returenrar x med de n bitarna som börjar
   vid position p inverterade, (dvs. ändrar 1 till 0 och vice versa), med de övriga
   oförändrade. */

#include <stdio.h>

unsigned invert(unsigned x, unsigned p, unsigned n)
{
	/* mask med ettor för de orörda positionerna. */
	unsigned mask = (~0 << p << n) | ~(~0 << p);

	return x & mask | x ^ ~mask;
}

main()
{
	printf("0xffff,0,8 ska blir 0xff00: %x\n", invert(0xffff, 0, 8));
	printf("0xffff,4,8 ska blir 0xf00f: %x\n", invert(0xffff, 4, 8));
	printf("b10101010, 2,3 ska bli b10010010: sant (1)? %i\n", invert(0b10101010, 2, 3) == 0b10110110);
	printf("0xAAAA,4,12 ska blir 0x555A: %x\n", invert(0xAAAA, 4, 12));

	return 0;
}
