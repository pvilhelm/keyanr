/* Skriv ett promgram som bestämmer talområdet för variabler av typ
 * char, short, int och long, både signed och unsigned, genom att 
 * skriva ut lämpliga värden från standardhuvudfiler och genom beräkningar.
 * Det blir en besvärligare uppgift om du beräknar dem: bestäm talområdet 
 * för de olika flyttalstyperna. */

#include <stdio.h>
#include <limits.h>
#include <float.h>

#define HARD

main()
{
#ifndef HARD
	printf("signed long   : %10li %10li\n", LONG_MIN, LONG_MAX);
	printf("signed int    : %10i %10i\n", INT_MAX, INT_MIN);
	printf("unsigned int  : %10u %10u\n", UINT_MAX, 0);
	printf("signed short  : %10i %10i\n", SHRT_MAX, SHRT_MIN);
	printf("unsigned short: %10u %10u\n", USHRT_MAX, 0);
	printf("signed char   : %10i %10i\n", SCHAR_MAX, SCHAR_MIN);
	printf("unsigned char : %10u %10u\n", UCHAR_MAX, 0);
	printf("float         : %g %g\n", FLT_MAX, FLT_MIN);
	printf("double        : %g %g\n", DBL_MAX, DBL_MIN);
	printf("long double   : %Lg %Lg\n", LDBL_MAX, LDBL_MIN);
#else
	/* Bestäm högsta schar. Jag har ju inte lärt mig bit-
	 * operander än så vi får lösa det på annat sätt. */
	signed char c0, c1 = 1;
	signed char c2, c3 = -1;

	while(1) {
		c0 = c1;
		c1 *= 2;
		if (c0 > c1) {
			/* scharen har slagit över. */
			c1--;
			break;
		}
	}
	while(1) {
		c2 = c3;
		c3 *= 2;
		if (c2 < c3) {
			/* scharen har slagit under */
			break;
		}
	}

	/* Orka jag tjuvläser kapitlet om bitoperander. */
	printf("unsigned char : %20i %20i\n",(unsigned char) ~0, 0);
	printf("signed char   : %20i %20i\n", c1, c2);

	printf("unsigned short: %20u %20u\n",(unsigned short)~0,0);
	printf("signed short  : %20i %20i\n",
	/*Max*/	(signed short)((unsigned short)~0 >>1),
	/*Min*/	(signed short)~((unsigned short)~0 >>1));

	printf("unsigned int  : %20u %20u\n", ~0, 0);
	printf("signed int    : %20i %20i\n",
		(int)((unsigned int)~0 >> 1),
		(int)~((unsigned int)~0 >> 1));

	printf("unsigned long : %20lu %20lu\n", ~0UL, 0UL);
	printf("signed long   : %20li %20li\n",
		(long)((unsigned long)~0 >> 1),
		(long)~((unsigned long)~0 >> 1));

	/* Verkar klurigt att hitta största floaten utan att
	   använda float.h:s funktioner ... återkommer till
	   när jag orkar. Ska man söka efter den på ngt smart 
	   sätt? */

#endif
	return 0;
}
