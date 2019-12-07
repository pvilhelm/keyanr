/* Skriv ett korsreferensprogram som skriver ut en lista av alla ord i ett dokument
 * och för varje ord en lista över de rader på vilka det förekommer.
 *
 * Ta bort onödiga ord som "den", "det", "och" med flera.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct word_list {
	struct word_list *next;
	char *word;
} word_list_t;

typedef struct tree_node {
	struct tree_node *left;
	struct tree_node *right;
	char * word;
	int count;
	word_list_t *wl;
} tree_node_t;

word_list_t *wl_calloc(void)
{
	return (word_list_t *) calloc(1, sizeof(word_list_t));
}

tree_node_t *tn_calloc(void)
{
	return (tree_node_t *) calloc(1, sizeof(tree_node_t));
}

word_list_t *add_word(word_list_t *wl, char *w)
{
	if (!wl) {
		wl = wl_calloc();
		wl->word = strdup(w);
		return wl;
	}

	/* Hitta slutet på listan om inte raden redan finns i listan. */
	while(wl->next) {
		wl = wl->next;
		if (strcmp(w, wl->word) == 0) {
			printf("Ordet finns redan i listan: %s", w);
			exit(1);
		}

	}

	wl->next = wl_calloc();
	wl->next->word = strdup(w);

	return wl;
}

tree_node_t *add_leaf(tree_node_t *tn, char *word)
{
	int scmp;
	if (!tn) {	/* Noden är tom */
		tn = tn_calloc();
		tn->word = strdup(word);
		tn->count++;
	} else if ((scmp = strcmp(word, tn->word)) == 0) {
		/* Detta är rätt nod. */
		tn->count++;
	} else if (scmp < 0) {
		tn->left = add_leaf(tn->left, word);
	} else
		tn->right = add_leaf(tn->right, word);
	return tn;
}

tree_node_t *add_leaf_count(tree_node_t *tn, char *word, int count)
{
	if (!tn) {	/* Noden är tom */
		tn = tn_calloc();
		tn->wl = add_word(tn->wl, word);
		tn->count = count;
	} else if (count == tn->count) {
		add_word(tn->wl, word);
	} else if (count < tn->count) {
		tn->left = add_leaf_count(tn->left, word, count);
	} else
		tn->right = add_leaf_count(tn->right, word, count);
	return tn;
}

/* Skriv ut trädet från a */
void print_tree(tree_node_t *tn)
{
	if (tn->right) print_tree(tn->right);

	printf("%d --", tn->count);
	word_list_t *wl = tn->wl;
	while(wl) {
		printf("%s, ", wl->word);
		wl = wl->next;
	}
	printf("\n");

	if (tn->left) print_tree(tn->left);

	return;
}

int is_valid_char(char c)
{
	if (isalnum(c)) return 1;
	else return 0;
}

char *filter_token(char *tkn)
{
	int len = strlen(tkn);
	char *tkn_f = strdup(tkn);

	/* Filtrerar bort "?",".", "\n" etc */
	for (int i = 0; i < len; i++) {
		if (!is_valid_char(tkn_f[i])) {
			tkn_f[i] = 0;
			return tkn_f;
		}
	}

	return tkn_f;
}

/* Kopierar ett träd sorterat på ord till ett träd sorterat på count. */
void copy_wtree_to_ctree(tree_node_t **ttn, tree_node_t *stn)
{
	if (!stn) return;

	if (!*ttn)
		*ttn = add_leaf_count(*ttn, stn->word, stn->count);
	else
		add_leaf_count(*ttn, stn->word, stn->count);

	copy_wtree_to_ctree(ttn, stn->left);
	copy_wtree_to_ctree(ttn, stn->right);

	return;
}

int main(int argc, char **argv)
{
	tree_node_t *p_word_root = 0;

	FILE *f;
	if (argc == 2) {
		f = fopen(argv[1],"r");
		if (!f) {
			printf("Kunde inte läsa in fil: %s", argv[1]);
			return 1;
		}
	} else
		f = stdin;

	int c;

	char *line = 0;
	size_t n = 0;
	int row_nr = 0;

	while ((c = getline(&line, &n, f)) > 0) {
		char *tkn = strtok(line," ");
		row_nr++;

		do {
			tkn = filter_token(tkn);
			if (*tkn) { /* Om ordet blev 0 så skippar vi lägga till det. */
				if (!p_word_root)
					p_word_root = add_leaf(p_word_root, tkn);
				else
					add_leaf(p_word_root, tkn);
			}
			free(tkn);
		} while(tkn = strtok(0, " "));

		free(line);
		line = 0;
		n = 0;
	}

	tree_node_t *p_count_root = 0;

	/* Gå igenom trädet och lägg det i ett nytt sorterat på "count" istället för "word" */
	copy_wtree_to_ctree(&p_count_root, p_word_root);

	print_tree(p_count_root);

	return 0;
}

