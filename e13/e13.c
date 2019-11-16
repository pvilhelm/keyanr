#include <stdio.h>

/* Skriv ut Fahrenheit-Celsius tabellen 
   för fahr = 0, 20, . . ., 300, flyttalsversion */

main()
{
	float fahr, celsius;
	int lower, upper, step;

	lower = 0;	/* nedre gräns för tabell */
	upper = 300; 	/* övre gräns */
	step = 20;	/* stegstorlek */
	
	printf("Fahrenhet och Celsius, tabell\n");
	printf("F    C\n");

	fahr = lower;
	while (fahr <= upper) {
		celsius = (5./9.) * (fahr - 32.);
		printf("%3.0f %6.1f\n", fahr, celsius);
		fahr = fahr + step;
	}

}
