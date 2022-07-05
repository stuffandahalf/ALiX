#include "_null.h"
#include "_size_t.h"

#ifndef __STDDEF_H
#define __STDDEF_H 1

#if defined(__LLP64__) && __STDC_VERSION__ >= 199901L
typedef signed long long int __ptrdiff_t;
typedef unsigned long long int __ptr_t;
#elif defined(__LP64__) || defined(__ILP64__)
typedef signed long int __ptrdiff_t;
typedef unsigned long int __ptr_t;
#elif defined(__ILP32__) || defined(__LP32__)
typedef signed long int __ptrdiff_t;
typedef unsigned long int __ptr_t;
#endif

typedef int wchar_t;

#define offsetof(type, member) ((size_t)(__ptr_t)(&((type *)NULL)->member))

#endif /* __STDDEF_H */
