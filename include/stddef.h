#ifndef __STDDEF_H
#define __STDDEF_H 1

#include "_null.h"
#include "_size_t.h"

typedef int wchar_t;

#define offsetof(type, member) ((size_t)(&((type *)NULL)->member))

#endif /* __STDDEF_H */