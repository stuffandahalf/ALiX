#include <stdlib.h>

#include "ht.h"

int ht_get(struct ht_node *root, const char *key)
{
	const char *c = key;
	struct ht_node *cur = root;

	for (c = key; *c != '\0'; c++) {
		if (!cur->children[*c]) {
			return -1;
		}
		cur = cur->children[*c];
	}

	return cur->value;
}

int ht_set(struct ht_node *root, const char *key, int value)
{
	int i;
	const char *c;
	struct ht_node *new, *cur = root;

	for (c = key; *c != '\0'; c++) {
		if (!cur->children[*c]) {
			new = malloc(sizeof(struct ht_node));
			if (!new) {
				return 1;
			}
			new->value = -1;
			for (i = 0; i < ASCII_RANGE; i++) {
				new->children[i] = NULL;
			}
			cur->children[*c] = new;
		}

		cur = cur->children[*c];
	}

	cur->value = value;

	return 0;
}

static void ht_free2(struct ht_node *root, int this)
{
	int i;
	
	if (!root) {
		return;
	}

	for (i = 0; i < ASCII_RANGE; i++) {
		if (root->children[i]) {
			ht_free2(root->children[i], 1);
		}
	}

	if (this) {
		free(root);
	}
}

void ht_free(struct ht_node *root)
{
	ht_free2(root, 0);
}

