#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <config.h>

int configure(int argc, char **argv);
int process(FILE *ifp, FILE *ofp);
void cleanup(void);

const char *ifname = NULL;
const char *ofname = NULL;

FILE *ifp, *ofp;
unsigned long ofsz = 0;

int pm_type = -1;

int main(int argc, char **argv)
{
	FILE *ifp, *ofp;

	setlocale(LC_ALL, "");
	atexit(cleanup);

	if (configure(argc, argv)) {
		return 1;
	}

	if (ifname) {
		ifp = fopen(ifname, "r");
		if (!ifp) {
			perror(NULL);
			return 1;
		}
	} else {
		ifp = stdin;
	}

	ofp = fopen(ofname, "a");
	if (!ofp) {
		perror(NULL);
		return 1;
	}

	if (process(ifp, ofp)) {
		return 1;
	}


	printf("Hello World!\n");
	return 0;
}

void
cleanup(void)
{
	if (ifp) {
		fclose(ifp);
	}
	if (ofp) {
		fclose(ofp);
	}
}

void copyright(char *name);
void usage(char *name);

int configure(int argc, char **argv)
{
	int c;

	while ((c = getopt(argc, argv, "chi:o:")) != -1) {
		switch (c) {
		case 's':
			/* script mode */
			break;
		case 't':
			/* image type */
			break;
		case 'c':
			/* copyright */
			copyright(argv[0]);
			break;
		case 'i':
			/* input file */
			ifname = optarg;
			break;
		case 'o':
			/* output file */
			ofname = optarg;
			break;
		case 'h':
		default:
			/* help */
			usage(argv[0]);
			return 1;
		}
	}

	if (!ofname) {
		fprintf(stderr, "Missing required argument '-o'\n");
		usage(argv[0]);
		return 1;
	}

	return 0;
}

void
copyright(char *name)
{
	printf("%s 1.0.0\n", name);
	printf("Copyright (C) 2021-2022 Gregory Norton <gregory.norton@me.com>\n");
	printf("This is free software; see the source for copying conditions."
		" There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR"
		" A PARTICULAR PURPOSE.\n");
}

void
usage(char *name)
{
	fprintf(stderr, "Usage: %s [options] output\n", name);
	fprintf(stderr, "Options:\n");
}

#define BUFFER_SIZE 500

int
process(FILE *ifp, FILE *ofp)
{
	char buffer[BUFFER_SIZE] = { 0 };
	char *bp;
	int i;
	int argc, argl = 0;
	char **argv = NULL;
	const struct command commands[] = {
		{ "size", 's', size },
		{ "format", 'f', format },
		{ "partition", 'p', NULL },
		{ "write", 'w', NULL }
	};
	size_t commandsc = sizeof(commands) / sizeof(struct command);

	while (fgets(buffer, BUFFER_SIZE, ifp)) {
		if ((bp = strchr(buffer, '\n'))) {
			*bp = '\0';
		}
		fprintf(stderr, "%s\n", buffer);

		argc = 0;
		bp = buffer;

		/* split arguments on spaces */
		for (bp = strtok(buffer, " "); bp != NULL; bp = strtok(NULL, " ")) {
			if (argc == argl) {
				argl += 2;
				argv = realloc(argv, sizeof(char *) * argl);
			}
			argv[argc] = bp;
			if ((bp = strchr(bp, '#'))) {
				*bp = '\0';
				break;
			}

			if (argv[argc] != bp) {
				argc++;
			}
		}

		if (!argc) {
			continue;
		}

		for (i = 0; i < commandsc; i++) {
			if (strlen(argv[0]) == 1 && argv[0][0] == commands[i].sname) {
				break;
			} else if (!strcmp(argv[0], commands[i].lname)) {
				break;
			}
		}

		if (i == commandsc) {
			fprintf(stderr, "Unrecognized command \"%s\"\n", argv[0]);
			return 1;
		}

		if (!commands[i].entry) {
			fprintf(stderr, "command \"%s\" not yet implemented\n", commands[i].lname);
			continue;
		}
		if (commands[i].entry(argc, argv)) {
			return 1;
		}
	}

	free(argv);

	return 0;
}

