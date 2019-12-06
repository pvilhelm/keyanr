/* Ändra sorteringsprogrammet till att hantera flagga -r,
 * vilken indikerar att stortering skall ske i omvänd
 * ordning. Förvissa dig om att -r fungerar tillsammans med -n.
 */

#include <stdio.h>
#include <string.h>

#define MAXLINES 5000
char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void qsort2(void *lineptr[], int left, int right, int (*comp)(void *, void *));

int numcmp(char *, char *);

int numrcmp(char *t, char *s)
{
	return -numcmp(t, s);
}

int my_strrcmp(const char *t, const char *s)
{
	return -strcmp(t,s);
}

int main(int argc, char *argv[])
{
	  int nlines;
	  int numeric = 0;
	  int reverse = 0;

	  for (int i = 1; i < argc; i++) {
		  if(strcmp(argv[i], "-n") == 0)
			  numeric = 1;
		  if(strcmp(argv[i], "-r") == 0)
			  reverse = 1;
	  }
	  printf("reverse: %i\n", reverse);
	  printf("numeric: %i\n", numeric);

	  if((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		  int (*cmp)(void*, void*);
		  if (reverse && numeric)
			  cmp = (int (*)(void*, void*))numrcmp;
		  else if (numeric)
			  cmp = (int (*)(void*, void*))numcmp;
		  else if (reverse)
			  cmp = (int (*)(void*, void*))my_strrcmp;
		  else
			  cmp = (int (*)(void*, void*))strcmp;

		  qsort2((void **) lineptr, 0, nlines-1, cmp);
		  writelines(lineptr, nlines);

		  return 0;

	  } else {
		  printf("input too big to sort\n");
		  return 1;
	  }
}

void qsort2(void *v[], int left, int right, int(*comp)(void *, void *))
{
  int i, last;
  void swap(void *v[], int, int);

  if(left >= right)
    return;
  swap(v, left, (left + right) / 2);
  last = left;
  for(i = left + 1; i <= right; i++)
    if((*comp)(v[i], v[left]) < 0)
      swap(v, ++last, i);
  swap(v, left, last);
  qsort2(v, left, last - 1, comp);
  qsort2(v, last + 1, right, comp);
}

#include <stdlib.h>

int numcmp(char *s1, char *s2)
{
  double v1, v2;

  v1 = atof(s1);
  v2 = atof(s2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return 0;
}

void swap(void *v[], int i, int j)
{
  void *temp;

  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}


#define MAXLEN 1000
int getline2(char *, int);
char *alloc(int);

int readlines(char *lineptr[], int maxlines)
{
  int len, nlines;
  char *p, line[MAXLEN];

  nlines = 0;
  while ((len = getline2(line, MAXLEN)) > 0)
    if (nlines >= maxlines || (p = alloc(len)) == NULL)
      return -1;
    else
      {
	line[len-1] = '\0';
	strcpy(p, line);
	lineptr[nlines++] = p;
      }
  return nlines;
}

int getline2(char *s, int lim)
{
  int c, i;

  for (i = 0; i<lim-1 && (c=getchar()) != EOF && c != '\n'; i++)
    s[i] = c;
  if (c == '\n')
    {
      s[i] = c;
      ++i;
    }
  s[i] = '\0';
  return i;
}

#define ALLOCSIZE 50000

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char *alloc(int n)
{
  if (allocbuf + ALLOCSIZE - allocp >= n)
    {
      allocp += n;
      return allocp - n;
    }
  else
    return 0;
}

void writelines(char *lineptr[], int nlines)
{
  int i;

  for (i = 0; i < nlines; i++)
    printf("%s\n", lineptr[i]);
}
