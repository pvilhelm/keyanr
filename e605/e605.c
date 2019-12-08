/* Skriv en funktion undef som tar bort ett namn och definition från
 * tabellen som upprätthålls av lookup och install.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct nlist {       /* table entry: */
   struct nlist *next;   /* next entry in chain */
   char *name;           /* defined name */
   char *defn;           /* replacement text */
};

#define BUG_PRINT_EXIT() do {printf("Bugg: %s, %s:%d", \
		__func__, __FILE__, __LINE__); exit(1);} while (0)

#define HASHSIZE 3

static struct nlist *hashtab[HASHSIZE];  /* pointer table */

/* hash:  form hash value for string s */
unsigned hash(char *s)
{
   unsigned hashval;

   for (hashval = 0; *s != '\0'; s++)
	   hashval = *s + 31 * hashval;
   return hashval % HASHSIZE;
}

/* lookup:  look for s in hashtab */
struct nlist *lookup(char *s)
{
   struct nlist *np;

   for (np = hashtab[hash(s)];  np != NULL; np = np->next)
	   if (strcmp(s, np->name) == 0)
		   return np;     /* found */
   return NULL;           /* not found */
}

/* install:  put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
   struct nlist *np;
   unsigned hashval;

   if ((np = lookup(name)) == NULL) { /* not found */
	   np = (struct nlist *) malloc(sizeof(*np));
	   if (np == NULL || (np->name = strdup(name)) == NULL)
		   return NULL;
	   hashval = hash(name);
	   np->next = hashtab[hashval];
	   hashtab[hashval] = np;
   } else       /* already there */
	   free((void *) np->defn);   /*free previous defn */
   if ((np->defn = strdup(defn)) == NULL)
	   return NULL;
   return np;
}

int undef(char *name)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(name)) == NULL) { /* not found */
		return 0;
	} else {      /* Hittades */
	   /* Hitta föräldern */
		hashval = hash(name);
		struct nlist *parent = hashtab[hashval];
		while(parent && parent->next != np)
			parent = parent->next;
		if (!parent) {
			hashtab[hashval] = 0; /* Noden är första noden i listan. */
		} else {
			parent->next = np->next; /* Koppla föräldern till nodens eget barn. */
		}

		free((void *) np->defn);
		free(np->name);
		free(np);
	}

	return 1;
}

int main(int argc, char **argv)
{
	/* Lägg till alla par av argument i hash-tabellen. */
	for (int i = 1; i + 1 < argc; i += 2) {
		install(argv[i], argv[i + 1]);
	}
	/* Slå upp alla poster och skriv ut dem. */
	for (int i = 1; i + 1 < argc; i += 2) {
		struct nlist *nl = lookup(argv[i]);
		if (nl)
			printf("Nyckel: %s, post: %s\n", argv[i], nl->defn);
		else
			printf("Nyckel: %s, hittades inte\n", argv[i]);
	}

	/* Ta bort var annan post */
	for (int i = 1; i + 1 < argc; i += 2) {
		undef(argv[i]);
	}

	/* Slå upp alla poster och skriv ut dem. */
	for (int i = 1; i + 1 < argc; i += 2) {
		struct nlist *nl = lookup(argv[i]);
		if (nl)
			printf("Nyckel: %s, post: %s\n", argv[i], nl->defn);
		else
			printf("Nyckel: %s, hittades inte\n", argv[i]);
	}
}


