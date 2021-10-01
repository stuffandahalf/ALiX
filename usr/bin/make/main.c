#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "ht.h"
#include "al.h"

#define FNAME_COUNT 6
const char *mkfnames[FNAME_COUNT] = {
	"./makefile",
	"./Makefile",
	"./s.makefile",
	"SCCS/s.makefile",
	"./s.Makefile",
	"SCCS/s.Makefile"
};

struct target {
	ARRAYLIST(const char *) results;
	ARRAYLIST(const struct target *) deps;
	ARRAYLIST(const char *) comms;
};

FILE *mkfile;

// hash map of targets
struct ht_node target_ht = { .value = -1, .children = { 0 } };
ARRAYLIST(struct target) targets = { .a = NULL, .sz = 0, .c = 0 };

// hash map of variables
struct ht_node var_ht = { .value = -1, .children = { 0 } };
ARRAYLIST(const char *) vars = { .a = NULL, .sz = 0, .c = 0 };

int parse(FILE *mkfile);

// free all held resources at application exit
void release(void)
{
	ht_free(&target_ht);
	fclose(mkfile);
}

int main(int argc, char **argv)
{
	int i;

	setlocale(LC_ALL, "");
	atexit(release);

	for (i = 0; i < FNAME_COUNT && mkfile == NULL; i++) {
		if (access(mkfnames[i], R_OK)) {
			continue;
		}
		mkfile = fopen(mkfnames[i], "r");
		if (!mkfile) {
			fprintf(stderr, "Failed to open \"%s\": %s\n", mkfnames[i],
				strerror(errno));
		}
	}
	if (!mkfile) {
		fprintf(stderr, "Failed to open makefile\n");
		return 1;
	}

	// set up hashtables
	

	// handle file here	
	if (parse(mkfile)) {
		fprintf(stderr, "Failed to parse makefile\n");
		return 1;
	}

/*	ht_set(&root, "three", 3);
	ht_set(&root, "twelve", 12);
	ht_set(&root, "twenty", 20);
	ht_set(&root, "hello", 33);

	printf("three: %d\n", ht_get(&root, "three"));
	printf("twelve: %d\n", ht_get(&root, "twelve"));
	printf("twenty: %d\n", ht_get(&root, "twenty"));
	printf("hello: %d\n", ht_get(&root, "hello"));

	ht_set(&root, "three", 52);
	printf("three: %d\n", ht_get(&root, "three"));*/

//	fclose(mkfile);
	return 0;
}

#define BUFFER_SZ (256)
int parse(FILE *mkfile)
{
	char buffer[BUFFER_SZ];
	char *start, *c;
	struct target *target = NULL;
	int cont_line = 0;

	while (fgets(buffer, BUFFER_SZ, mkfile)) {
		c = buffer;
		if (*c == '\0' || *c == '\n') {
			continue;
		}

/*		if (cont_line) {
			while (isspace(*c++));
		}*/

		if (!isspace(*c)) {
			printf("%s\n", buffer);
			/* new target */

			start = c;
			/*while (!isspace(*c) && *c != ':') {
				// find next space
			}*/
			
		} else {
			/* new command */
			while (isspace(*c) && *c) c++;
			if (!*c) {
				continue;
			}
			printf("%s\n", c);
		}
	}

	return 0;
}

