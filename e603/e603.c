/* Skriv ett korsreferensprogram som skriver ut en lista av alla ord i ett dokument
 * och för varje ord en lista över de rader på vilka det förekommer.
 *
 * Ta bort onödiga ord som "den", "det", "och" med flera.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct row_list {
	struct row_list *next;
	int row_nr;
} row_list_t;

typedef struct tree_node {
	struct tree_node *left;
	struct tree_node *right;
	char * word;
	row_list_t *row_list;
} tree_node_t;

tree_node_t *p_root;

row_list_t *rl_calloc(void)
{
	return (row_list_t *) calloc(1, sizeof(row_list_t));
}

tree_node_t *tn_calloc(void)
{
	return (tree_node_t *) calloc(1, sizeof(tree_node_t));
}

row_list_t *add_row(row_list_t *rl, int row_nr)
{
	/* Hitta slutet på listan om inte raden redan finns i listan. */
	while(rl->next) {
		rl = rl->next;
		if (row_nr == rl->row_nr)
			return 0; /* Raden finns redan i listan. */
	}

	rl->next = rl_calloc();
	rl->next->row_nr = row_nr;

	return rl->next;
}

tree_node_t *add_leaf(tree_node_t *tn, char *word, int row_nr)
{
	int scmp;
	if (!tn) {	/* Noden är tom */
		tn = tn_calloc();
		tn->row_list = rl_calloc();
		tn->row_list->row_nr = row_nr;
		tn->word = strdup(word);
	} else if ((scmp = strcmp(word, tn->word)) == 0) {
		/* Detta är rätt nod. */
		add_row(tn->row_list, row_nr);
	} else if (scmp < 0) {
		tn->left = add_leaf(tn->left, word, row_nr);
	} else
		tn->right = add_leaf(tn->right, word, row_nr);
	return tn;
}

/* Skriv ut trädet från a */
void print_tree(tree_node_t *tn)
{
	if (tn->left) print_tree(tn->left);

	printf("%s -- ", tn->word);
	row_list_t *rl = tn->row_list;
	while(rl) {
		printf("%d, ", rl->row_nr);
		rl = rl->next;
	}
	printf("\n");

	if (tn->right) print_tree(tn->right);

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

int main(int argc, char **argv)
{
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
			if (!p_root)
				p_root = add_leaf(p_root, tkn, row_nr);
			else
				add_leaf(p_root, tkn, row_nr);
			free(tkn);
		} while(tkn = strtok(0, " "));

		free(line);
		line = 0;
		n = 0;
	}

	print_tree(p_root);

	return 0;
}

