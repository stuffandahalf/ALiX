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
	ARRAYLIST(const char *) deps;
	ARRAYLIST(const char *) comms;
};

FILE *mkfile;
struct {
	int print: 1;
} opts = {
	.print = 0
};

// hash map of targets
struct ht_node targets_ht = { .value = -1, .children = { 0 } };
ARRAYLIST(struct target) targets = { .a = NULL, .sz = 0, .c = 0 };

// hash map of macros
struct ht_node macros_ht = { .value = -1, .children = { 0 } };
ARRAYLIST(const char *) macros = { .a = NULL, .sz = 0, .c = 0 };

// list of targets to build
ARRAYLIST(const char *) build_targets = { .a = NULL, .sz = 0, .c = 0 };

int configure(int argc, char **argv);
int parse(FILE *mkfile);
void emit(void);
int build(const char *target_name);

// free all held resources at application exit
void release(void)
{
	int i;
	
	AL_FREE(build_targets);
	
	AL_FREE(macros);
	ht_free(&macros_ht);
	
	for (i = 0; i < targets.c; i++) {
		AL_FREE(targets.a[i].artifacts);
		AL_FREE(targets.a[i].deps);
		AL_FREE(targets.a[i].comms);
	}
	AL_FREE(targets);
	ht_free(&targets_ht);
	
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
	
	if (opts.print) {
		emit();
		return 0;
	}

	for (i = 0; i < build_targets.c; i++) {
		if (build(build_targets.a[i])) {
			fprintf(stderr, "Failed to build target \"%s\"\n",
				build_targets.a[i]);
			return 1;
		}
	}

	return 0;
}

int configure(int argc, char **argv)
{
	int i, c;
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
			opts.print = 1;
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
	
	/*for (i = optind; i < argc; i++) {
		printf("%d\t", i);
		printf("%s\n", argv[i]);
	}*/
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
	char *c;
	ARRAYLIST(char) term = { .a = NULL, .c = 0, .sz = 0 };
	struct target *tgt = NULL;
	int state = TARGET_STATE_ARTIFACT;

	while (fgets(buffer, BUFFER_SZ, mkfile)) {
		c = buffer;
		if (*c == '\0' || *c == '\n') {
			continue;
		}

		// if not a line continuation and line does not begin with a space
		if (!(state & TARGET_STATE_CONTLN)) {
			if (isspace(*c)) {
				state |= TARGET_STATE_COMMS;
				while (isspace(*c)) {
					c++;
				}
				term.a = NULL;
				term.sz = 0;
				term.c = 0;
				for (; *c != '\n' && *c != '\0'; c++) {
					AL_APPEND(char, term, *c);
				}
				if (term.c) {
					AL_APPEND(char *, tgt->comms, term.a);
				}
			} else {
				printf("%s\n", buffer);
				/* new target */
				if (targets.c == targets.sz) {
					targets.sz += 3;
					targets.a = realloc(targets.a,
						sizeof(struct target) * targets.sz);
				}
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
		}
	
		// populate tgt by parsing buffer
		// use state to track
		for (; *c != '\0'; c++) {
			if (isspace(*c) && term.c == 0) {
				continue;
			}
			if ((isspace(*c) && *c != '\n') || (!(state & TARGET_STATE_ESCAPE) && *c == ':')) {
				//*c = '\0';
				AL_APPEND(char, term, '\0');
				//target_mod(tgt, start, state);
				if (state & TARGET_STATE_COMMS) {
					AL_APPEND(char *, tgt->comms, term.a);
				} else if (state & TARGET_STATE_DEPS) {
					AL_APPEND(char *, tgt->deps, term.a);
				} else if (state & TARGET_STATE_ARTIFACT) {
					AL_APPEND(char *, tgt->artifacts, term.a);
				}
				term.a = NULL;
				term.c = 0;
				term.sz = 0;
			} else if (*c == '\\') {
				state |= TARGET_STATE_ESCAPE;
			}
			
			if (*c == ':' && !(state & TARGET_STATE_ESCAPE)) {
				state |= TARGET_STATE_DEPS;
			} else {
				AL_APPEND(char, term, *c);
			}
		}
	}
	
	AL_FREE(term);

	return 0;
}

void emit(void) {
	int i, j;
	
	printf("MACROS\n");
	
	printf("TARGETS\n");
	for (i = 0; i < targets.c; i++) {
		if (i) {
			printf("\n");
		}
		
		for (j = 0; j < targets.a[i].artifacts.c; j++) {
			if (j) {
				printf(" ");
			}
			printf("%s", targets.a[i].artifacts.a[j]);
		}
		printf(": ");
		for (j = 0; j < targets.a[i].deps.c; j++) {
			if (j) {
				printf(" ");
			}
			printf("%s", targets.a[i].deps.a[i]);
		}
		printf("\n");
		for (j = 0; j < targets.a[i].comms.c; j++) {
			printf("\t%s\n", targets.a[i].comms.a[j]);
		}
		printf("\n");
	}
}

int build(const char *target_name)
{
	
}
