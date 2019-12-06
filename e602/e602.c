/* Skriv ett program som läser ett c-program och i alfabetisk ordning
 * skriver ut varje grupp av variabelnamn, som är identiska i de 6 första
 * positionerna, men skiljer sig åt i en senare position. Räkna inte ord
 * i strängar eller kommentarer. Gör 6 till en parameter som kan sättas
 * från kommandoraden. */


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

struct wnode {
	char *word;
	struct wnode *next;
};

struct tnode {     /* the tree node: */
   struct wnode *wnode;  /* points to the text list */
   char *identifer;
   int count;            /* number of occurrences */
   struct tnode *left;   /* left child */
   struct tnode *right;  /* right child */
};


struct key {
    char *word;
    int count;
} keytab[] = {
    "auto", 0,
    "break", 0,
    "case", 0,
    "char", 0,
    "const", 0,
    "continue", 0,
    "default", 0,
	"double", 0,
	"float",0,
	"int",0,
    /* ... */
	"return", 0,
    "unsigned", 0,
    "void", 0,
    "volatile", 0,
    "while", 0
};

#define NKEYS (sizeof keytab / sizeof(struct key))

#define MAXWORD 100
struct tnode *addtree(struct tnode *p, char *w, char *i);
void treeprint(struct tnode *);
int getword(char *, int);
int binsearch(char *, struct key *, int);

/* word frequency count */
int main(int argc, char **argv)
{
	int n_letters = 6;
	if (argc == 2) {
		n_letters = atoi(argv[1]);
		if (!n_letters) {
			printf("Ogitligt antal tecken angivet. \"%s\".\n", argv[1]);
			exit(1);
		}
	}

   struct tnode *root;
   char word[MAXWORD];

   root = NULL;
   while (getword(word, MAXWORD) != EOF)
	   if (isalpha(word[0]))
		   if (binsearch(word, keytab, NKEYS) < 0) {/* Kolla om så att det inte är ett keyword. */
			   /* Vi vill bara jämföra på de n första bokstäverna. */
			   char *id = (char*) calloc(n_letters + 1, sizeof(char));
			   strncpy(id, word, n_letters);
			   id[n_letters] = 0;
			   root = addtree(root, word, id);
			   free(id); /* Addtree gör en egen kopia om det behövs. */
		   }
   treeprint(root);
   return 0;
}

struct wnode *walloc(void);
struct tnode *talloc(void);


/* addtree:  add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w, char *i)
{
   int cond;

   if (p == NULL) {     /* a new word has arrived */
	   p = talloc();    /* make a new node */
	   p->wnode = walloc();
	   p->wnode->word = strdup(w);
	   p->count = 1;
	   p->identifer = strdup(i);
   } else if ((cond = strcmp(i, p->identifer)) == 0) {
	   p->count++;      /* repeated identifier */
	   struct wnode *ptr = p->wnode;

	   if (!ptr) {printf("bugg");exit(1);}

	   int exists = 0;
	   while(!(exists = (strcmp(ptr->word, w) == 0))) {
		   if (ptr->next)
			   ptr = ptr->next;
		   else
			   break;
	   }
	   if (!exists) {
		   ptr->next = walloc();
	   	   ptr->next->word = strdup(w);
	   }
   } else if (cond < 0)   /* less than into left subtree */
	   p->left = addtree(p->left, w, i);
   else             /* greater than into right subtree */
	   p->right = addtree(p->right, w, i);
   return p;
}

void wnodeprint(struct wnode *n);
/* treeprint:  in-order print of tree p */
void treeprint(struct tnode *p)
{
   if (p != NULL) {
	   treeprint(p->left);
	   printf("%4d ---\n", p->count);
	   wnodeprint(p->wnode);
	   treeprint(p->right);
   }
}

void wnodeprint(struct wnode *n)
{
	while(n) {
		printf("\t%s\n", n->word);
		n = n->next;
	}
}


/* talloc:  make a tnode */
struct tnode *talloc(void)
{
   return (struct tnode *) calloc(1, sizeof(struct tnode));
}

/* walloc: gör en nod den länkade listan av namn. */
struct wnode *walloc(void)
{
	return (struct wnode *) calloc(1, sizeof(struct wnode));
}

#define BUFSIZE 100

char buf[BUFSIZE];    /* buffer for ungetch */
int bufp = 0;         /* next free position in buf */

int getch(void)  /* get a (possibly pushed-back) character */
{
   return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)   /* push character back on input */
{
   if (bufp >= BUFSIZE)
	   printf("ungetch: too many characters\n");
   else
	   buf[bufp++] = c;
}

/* getword:  get next word or character from input */
int getword(char *word, int lim)
{
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    while (isspace(c = getch()))
        ;
    if (c != EOF)
        *w++ = c;

    /* Ta bort kommentarer */
	if (c == '/') {
		char cc = getch();
		if (cc == '*') {
			*w = 0;
			while((c = getch()) != EOF) {
				if (c == '*') {
					cc = getch();
					if (cc == '/') {
						/* Hittade slutet på kommentaren. */
						return cc;
					} else if (cc == EOF) return EOF;
				}
				if (c == EOF) return EOF;
			}
			if (c == EOF) return EOF;
		} else
			ungetch('*');
	}

	/* Ta bort strängar */
	if (c == '"') {
		int is_escaped = 0;
		while((c = getch()) != EOF) {
			if (!is_escaped && c == '"')
				return c;
			else if (is_escaped)
				is_escaped = 0;
			else if (c == '\\')
				is_escaped = 1;
		}
		if (c == EOF) return EOF;
	}
	/* Ta bort char literals */
	if (c == '\'') {
		int is_escaped = 0;
		while((c = getch()) != EOF) {
			if (!is_escaped && c == '\'')
				return c;
			else if (is_escaped)
				is_escaped = 0;
			else if (c == '\\')
				is_escaped = 1;
		}
		if (c == EOF) return EOF;
	}

    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }


    for ( ; --lim > 0; w++)
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}


/* binsearch:  find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n)
{
   int cond;
   int low, high, mid;

   low = 0;
   high = n - 1;
   while (low <= high) {
	   mid = (low+high) / 2;
	   if ((cond = strcmp(word, tab[mid].word)) < 0)
		   high = mid - 1;
	   else if (cond > 0)
		   low = mid + 1;
	   else
		   return mid;
   }
   return -1;
}


