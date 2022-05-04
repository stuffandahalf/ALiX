#ifndef __ALIX_LIBC_REGEX_H
#define __ALIX_LIBC_REGEX_H 1

#include "_std.h"

/* include size_t */
#include <sys/types.h>

typedef regoff_t off_t;

typedef struct regmatch_t {
	regoff_t rm_so;		/* string offset of match */
	regoff_t rm_eo;		/* string offset of first char after match */
} regmatch_t;

/* constants used by regcomp() */
#define REG_EXTENDED	1
#define REG_ICASE		2
#define REG_NOSUB		4
#define REG_NEWLINE		8

/* constants used by regexec() */
#define REG_NOTBOL		1
#define REG_NOTEOL		2

/* return values */
#define REG_NOMATCH		1	/* regexec() failed to match() */
#define REG_BATPAT		2	/* invalid regular expression */
#define REG_ECOLLATE	3	/* invalid collating element referenced */
#define REG_ECTYPE		4	/* invalid character class type referenced */
#define REG_EESCAPE		5	/* trailing '\' in pattern */
#define REG_ESUBREG		6	/* number in \digit invalid or in error */
#define REG_EBRACK		7	/* "[]" imbalance */
#define REG_EPAREN		8	/* "(\)" or "()" imbalance */
#define REG_EBRACE		9	/* "\{\} imbalance */
#define REG_BADBR		10	/* content of "\{\}" invalid */
#define REG_ERANGE		11	/* invalid endpoint in range expression */
#define REG_ESPACE		12	/* out of memory */
#define REG_BADRPT		13	/* '?', '*', '+' not preceded by valid regular expression */
#define REG_ENOSYS		14	/* reserved */

extern int regcomp(regex_t *RESTRICT, const char *RESTRICT, int);
extern size_t regerror(int, const regex_t *RESTRICT, char *RESTRICT, size_t);
extern int regexec(const regex_t *RESTRICT, const char *RESTRICT, size_t, regmatch_t[RESTRICT], int);
void regfree(regex_t *);

#endif /* __ALIX_REGEX_H */
