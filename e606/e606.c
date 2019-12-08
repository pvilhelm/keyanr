/* Implementera en enkel version av #define-processorn (dvs utan argument), som
 * är lämplig att använda för C-program, baserad på rutinerna i det här avsnittet.
 *
 * Du kanske också får användning för getch och ungetch.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct nlist {       /* table entry: */
   struct nlist *next;   /* next entry in chain */
   char *name;           /* defined name */
   char *defn;           /* replacement text */
};

#define BUG_PRINT_EXIT_MSG(msg) do {printf("Bugg: %s, %s, %s:%d", \
		(msg), __func__, __FILE__, __LINE__); exit(1);} while (0)
#define BUG_PRINT_EXIT() BUG_PRINT_EXIT_MSG("")

#define HASHSIZE 3

static struct nlist *hashtab[HASHSIZE];  /* pointer table */

int getch(void);
void ungetch(int c);

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

void skip_comment(void)
{
	/* "/" och "*" är redan uppätna, så nu letar vi efter "*" "/" och inget
	 * av tecknena mellan dessa ska till stdout.
	 */
	int star = 0;
	char c;

	while((c = getch()) != EOF) {
		if (star) {
			if (c == '/')
				return;
			else if (c == '*')
				continue;
			else
				star = 0;
		} else {
			if (c == '*')
				star = 1;
		}
	}
	/* En fil kan inte sluta i en kommentar. */
	BUG_PRINT_EXIT_MSG("Filen slutar med inne i en kommentar.");
}

void skip_string(void)
{
	int escaped = 0;
	char c;

	putchar('"');

	while((c = getch()) != EOF) {
		putchar(c);
		if (escaped) {
			escaped = 0;
		} else {
			if (c == '\\')
				escaped = 1;
			else if (c == '"')
				return;
		}
	}
	BUG_PRINT_EXIT_MSG("Filen slutar inne i en string literal.");
}

void skip_charliteral(void)
{
	int escaped = 0;
	char c;

	putchar('\'');

	while((c = getch()) != EOF) {
		putchar(c);
		if (escaped) {
			escaped = 0;
		} else {
			if (c == '\\')
				escaped = 1;
			else if (c == '\'')
				return;
		}
	}
	BUG_PRINT_EXIT_MSG("Filen slutar inne i en char literal.");
}


void process_macro(void)
{
	char buffer[8];
	char symbol[128];
	char def[512];

	int i = 0;
	/* Ett # har ätits upp. */
	char c;

	/* Det finns undef, ifdef, ifndef, define, if, elif, endif, else och pragma som kan komma efter.
	 *
	 * Men för denna så kollar vi bara efter define och ignorerar allt annat (de skrivs inte ut till stdout).
	 */
	while((c = getch()) != EOF && i < 6) {
		if (isalpha(c))
			buffer[i++] = c;
		else
			break;
	}
	buffer[i] = 0;

	if (strcmp(buffer, "define") == 0) {
		/* Ät upp ws */
		while((c = getch()) != EOF && isspace(c))
			;
		if (c == EOF) return;
		/* Nu borde vi symbolen. */
		i = 0;
		symbol[i++] = c; /* Sista symbolen i ws-loopen ska med i symbolen */
		while((c = getch()) != EOF && isalnum(c) && i < 127) {
			symbol[i++] = c;
		}
		symbol[i++] = 0;
		if (c == EOF) return;

		/* Läs in resten i def. Escapade nl gör att raden fortsätter ... */
		i = 0;
		int is_escaped;
		while((c = getch()) != EOF && i < 511) {
			if (is_escaped)
				is_escaped = 0;
			else if (c == '\\')
				is_escaped = 1;
			else if (c == '\n')
				break; /* Slut på makrot. */
			def[i++] = c;
		}
		def[i++] = 0;
		if (c == EOF) return;

		/* TODO: Egentligen borde potentiella makron i markot expanderas här men orka ... */

		struct nlist *np;
		np = lookup(symbol);

		if (!np) { /* Lägg till macrot i listan. */
			install(symbol, def);
			return;
		} else
			return;
		/* TODO: Kolla att de olika definitioneran är identiska förutom ws */

	} else { /* Ät upp tecken till nl. */
		int is_escaped = 0;
		while((c = getch()) != EOF) {
			if (is_escaped)
				is_escaped = 0;
			else if (c == '\\')
				is_escaped = 1;
			else if (c == '\n')
				break; /* Slut på makrot. */
		}
	}

	return;
}

void process_token(char *tkn)
{
	/* Se om toknet är ett makro. */
	 struct nlist *np;

	 np = lookup(tkn);

	 if (!np) {
		printf("%s", tkn);
	 } else {
		printf("%s", np->defn);
	 }

	 return;
}

int main(int argc, char **argv)
{
#define BUFFER_SIZE 1024
	char buffer[BUFFER_SIZE];

	char c;
	char cc;
	int i;

	while((c = getch()) != EOF) {

		switch(c) {
		case '\r':
		case '\v':
		case ' ':
		case '\t':
		case '\n':
			/* Skriv olika slags ws rakt ut till stout. */
			putchar(c);
			break;
		case '#':
			process_macro();
			break;
		case '"':
			skip_string();
			break;
		case '\'':
			skip_charliteral();
			break;
		case '>':
		case '~':
		case '+':
		case '-':
		case '!':
		case '<':
		case '=':
		case '*':
		case '^':
		case '&':
		case ';':
		case ':':
		case '{':
		case '}':
		case '(':
		case ')':
		case '[':
		case ']':
			putchar(c);
			break;
		case '/':
			cc = getch();
			if (cc == '*')
				skip_comment();
			else {
				ungetch(cc);
				putchar(c);
			}
			break;
		default:
			/* Nu ska det vara ngt slags alphanumerisk token. */
			i = 0;
			buffer[i++] = c;
			get_char:
			if (i >= BUFFER_SIZE - 1) { /* Måste ha plats för 0 också*/
				BUG_PRINT_EXIT();
			}
			cc = getch();
			if (isalnum(cc)) {
				buffer[i++] = cc;
				goto get_char;
			} else if (cc == '_') {
				buffer[i++] = '_';
				goto get_char;
			} else {
				ungetch(cc); /* Lägg tillbaka icke-symbol-tecknet. */
			}
			buffer[i++] = 0;
			process_token(buffer);
			break;
		}
	}
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

