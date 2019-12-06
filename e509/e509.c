/* Skriv om day_of_year och month_day med pekare istället för med index. */

#include <stdlib.h>
#include <stdio.h>

static char daytab[2][13] = {
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};
/* day_of_year: set day of year from month & day */

int day_of_year(int year, int month, int day)
{
	if (month < 1 || month > 12) {
		printf("Månad felaktig: %d\n", month);
		return -1;
	}
	if (day < 1 || day > 31) {
		printf("Dag felaktig: %d\n", month);
		return -1;
	}

	int i, leap;
	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;

	if (daytab[leap][month] < day) {
		printf("Dag felaktig: %d. Kan vara högst %d.\n", month, daytab[leap][month]);
		return -1;
	}

	char *dt = &daytab[leap][1];
	for (int i = 1; i < month; i++)
		day += *dt++;
	return day;
}

/* Orka denna också. */
void month_day(int year, int yearday, int *pmonth, int *pday)
{
	int i, leap;
	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;

	if (yearday < 1 || yearday > (leap ? 366 : 365)) {
		printf("Felaktig dag på året: %d\n", yearday);
		*pmonth = 0;
		*pday = 0;
		return;
	}

	for (i = 1; yearday > daytab[leap][i]; i++)
		yearday -= daytab[leap][i];
	*pmonth = i;
	*pday = yearday;
}

int main(int argc, char **argv)
{
	for (int i = 1; i + 2 < argc; i += 3) {
		int doy = day_of_year(atoi(argv[i]),atoi(argv[i + 1]),atoi(argv[i + 2]));
		printf("%d\n", doy);
		int m;
		int d;
		month_day(atoi(argv[i]), doy, &m, &d);
		printf("%d %d %d\n", atoi(argv[i]), m, d);
	}
}
