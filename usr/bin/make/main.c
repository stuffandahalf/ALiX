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
	ARRAYLIST(const char *) artifacts;
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

int configure(int argc, char **argv);
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

	if (configure(argc, argv)) {
		return 1;
	}

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

int configure(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, "ef:hiknpqrSst")) != -1) {
		switch (c) {
		case 'e':
			break;
		case 'f':
			break;
		case 'i':
			break;
		case 'k':
			break;
		case 'n':
			break;
		case 'p':
			break;
		case 'q':
			break;
		case 'r':
			break;
		case 'S':
			break;
		case 's':
			break;
		case 't':
			break;
		case 'h':
		case '?':
		default:
			fprintf(stderr, "%s [-e] [-f makefile] [-i] [-k | -S] [-n] [-p] [-q] [-r] [-s] [-t] [MACRO=value...] [target...]\n", argv[0]);
			return 1;
			break;
		}
	}
	return 0;
}

#define BUFFER_SZ (256)

#define TARGET_STATE_ARTIFACT	1
#define TARGET_STATE_DEPS		2
#define TARGET_STATE_COMMS		4
#define TARGET_STATE_ESCAPE		8
#define TARGET_STATE_CONTLN		16
int parse(FILE *mkfile)
{
	static char buffer[BUFFER_SZ];
	char *start, *c;
	struct target *tgt = NULL;
	int state = TARGET_STATE_ARTIFACT;

	while (fgets(buffer, BUFFER_SZ, mkfile)) {
		c = buffer;
		if (*c == '\0' || *c == '\n') {
			continue;
		}

		// if not a line continuation and line does not begin with a space
		if (!(state & TARGET_STATE_CONTLN) && !isspace(*c)) {
			printf("%s\n", buffer);
			/* new target */
			if (targets.c == targets.sz) {
				targets.sz += 3;
			}
			targets.a = realloc(targets.a, sizeof(ARRAYLIST(struct target)) * targets.sz);
			tgt = &targets.a[targets.c++];

			// initialize fields of target
			tgt->artifacts.a = NULL;
			tgt->artifacts.sz = 0;
			tgt->artifacts.c = 0;

			tgt->deps.a = NULL;
			tgt->deps.sz = 0;
			tgt->deps.c = 0;

			tgt->comms.a = NULL;
			tgt->comms.sz = 0;
			tgt->comms.c = 0;

			state = TARGET_STATE_ARTIFACT;
		}
	
		// populate tgt by parsing buffer
		// use state to track 
	}

	return 0;
}

