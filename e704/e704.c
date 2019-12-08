/* Skriv en version av minprintf så att programmet kan hantera fler
 * av printfs möjligheter.
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>

/* minprintf: minimal printf with variable argument list */
int minscanf(char *fmt, ...)
{
   char buffer[512];
   int idx = 0;
   va_list ap; /* points to each unnamed arg in turn */
   char *p, *sval;
   int *ival;
   double *dval;
   unsigned int *uval;
   int n = 0;
   char c;

   va_start(ap, fmt); /* make ap point to 1st unnamed arg */
   for (p = fmt; *p; p++) {
	   if (*p != '%') {
		   continue;
	   }
	   while((c = getchar()) != EOF && isspace(c))
		   ;
	   if (c == EOF) return n;
	   idx = 0;
	   buffer[idx++] = c;
	   switch (*++p) {
	   case 'i':
		   ival = va_arg(ap, int*);
		   while((c = getchar()) != EOF && isdigit(c))
			   buffer[idx++] = c;
		   buffer[idx] = 0;
		   *ival = atoi(buffer);
		   n++;
		   if (c == EOF) return n;
		   break;
	   case 'f':
		   dval = va_arg(ap, double*);
		   while((c = getchar()) != EOF && (isdigit(c) || c == '.' || tolower(c) == 'e'))
			   buffer[idx++] = c;
		   buffer[idx] = 0;
		   *dval = atof(buffer);
		   n++;
		   if (c == EOF) return n;
		   break;
	   default:
		   break;
	   }
   }
   va_end(ap); /* clean up when done */

   return n;
}

int main(int argc, char **argv)
{
	int i; double f;
	minscanf("%i %f",&i, &f);

	printf("%i %f",i,f);

	return 0;
}
