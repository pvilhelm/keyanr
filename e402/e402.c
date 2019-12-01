/* Utvidga atof till att hantera exponentnotation på formen
 * 123.46e-6
 * där ett flyttal kan följas av e eller E och en eventuell
 * exponenet med tecken.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

double atof_my(char s[])
{
	double val, power, val_diff = 0.;
	int i, sign;

	for (i = 0; isspace(s[i]); i++)
		;

	sign = (s[i] == '-') ? -1 : 1;

	if (s[i] == '+' || s[i] == '-')
		i++;

	for (val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');

	if (s[i] == '.')
		i++;
	for (power = 1.0; isdigit(s[i]); i++) {
		/* Kahans algoritm. Var inte det som gav dålig precision
		 * utan att dela med 10or längre ner.
		 */
		power *= 10.0;
		double add = (s[i] - '0') / power - val_diff;
		double tmp = val + add;
		val_diff = (tmp - val) - add;
		val = tmp;
	}

	if (s[i] == 'e' || s[i] == 'E') {
		i++;
		int sign_e = s[i] == '-' ? -1 : 1;

		if (s[i] == '+' || s[i] == '-')
			i++;

		double val_e = 0.;
		for (val_e = 0.0; isdigit(s[i]); i++) {
			val_e = 10. * val_e + (s[i] - '0');
		}

		double e = sign_e * val_e;

		/* Exponenten e är positiv ska power delas,
		 * annars gångras (för vi delar allt med power
		 * senare).
		 */
		power = 1;
		if (e > 0) {
			for (int j = 0; j < e; j++) {
				power *= 10; /* Orginalversion hade /= vilket är dumt. */
			}
		} else if (e < 0)
			for (int j = 0; j < -e; j++)
				power *= 10;
		else { /* x^0 == 1 */
			val = 1.;
			power = 1.;
		}

		if (e < 0)
			return sign * val / power;
		else
			return sign * val * power;

	} else
		return sign * val;
}


int main(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		printf("%s -> (std) %g -> (min) %g\n", argv[i], atof(argv[i]), atof_my(argv[i]));
	}

	return 0;
}
