/* Vår binära sökning gör två tester inuti slingan, när en skulle räcka (till priset
 * av fler tester utanför). Skriv en version med bara en test inuti slingan
 * och mät tidsskillnaden vid exekveringen.
 */

#include <stdio.h>
#include <stdlib.h>

int binsearch(int x, int v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low+high) / 2;
		if (x < v[mid])
			high = mid - 1;
		else if (x > v[mid])
			low = mid + 1;
		else
			return mid;
	}

	return -1;
}

/* Denna är typ 5% långsammare. */
int binsearch2(int x, int v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;

	mid = (low+high) / 2;
	while (low <= high && x != v[mid]) {
		if (x < v[mid])
			high = mid - 1;
		else
			low = mid + 1;
		mid = (low+high) / 2;
	}

	return v[mid] == x ? mid : -1;
}

/* MKt långsammare 50% ca */
int binsearch3(int x, int v[], int n)
{
  int low, high, mid;

  low = -1;
  high = n;
  while (low + 1 < high) {
    mid = (low + high) / 2;
    if (v[mid] < x)
      low = mid;
    else
      high = mid;
  }
  if (high == n || v[high] != x)
    return -1;
  else
    return high;
}


/* Se till att inte optimeringen gör ngt fuffens. */
extern int quite_random_arr[];
extern int size_arr;
extern int look_for_arr[];

int main(int argc, char **argv)
{
	if (argc < 3) {printf("Ange antal tester och algoritm 1(gamla) eller 2(nya)... "); return -1;}

	printf("%s",argv[0]);
	int n = atoi(argv[1]);
	int alg = atoi(argv[2]);

	int found = 0;

	for (int j = 0; j < n; j++)
		for (int i = 0; i < size_arr; i++)
			if (alg == 1) {
				if (binsearch(look_for_arr[i], quite_random_arr, size_arr) != -1)
					found++;
			} else if (alg == 2) {
				if (binsearch2(look_for_arr[i], quite_random_arr, size_arr) != -1)
									found++;
			} else if (alg == 3) {
				if (binsearch3(look_for_arr[i], quite_random_arr, size_arr) != -1)
									found++;
			}

	return found;
}

