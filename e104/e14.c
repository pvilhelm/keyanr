#include <stdio.h>

/* Skriv ut Fahrenheit-Celsius tabellen 
   för cels = 0, 20, . . ., 300, flyttalsversion */

main()
{
	float fahr, celsius;
	int lower, upper, step;

	lower = 0;	/* nedre gräns för tabell */
	upper = 300; 	/* övre gräns */
	step = 20;	/* stegstorlek */
	
	printf("Fahrenhet och Celsius, tabell\n");
	printf("C    F\n");

	celsius = lower;
	while (celsius <= upper) {
		fahr = (celsius + 32.)*9./5.;
		printf("%3.0f %6.1f\n", celsius, fahr);
		celsius = celsius + step;
	}

}
