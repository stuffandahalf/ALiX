#ifndef HAMT_H
#define HAMT_H 1

#define ASCII_RANGE 128

struct ht_node {
	//char c;
	struct ht_node *children[ASCII_RANGE];
	int value;
};

int ht_get(struct ht_node *root, const char *key);
int ht_set(struct ht_node *root, const char *key, int value);
void ht_free(struct ht_node *root);

#endif /* HAMT_H */

