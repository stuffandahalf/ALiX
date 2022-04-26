#ifndef __SIZE_T_H
#define __SIZE_T_H 1

#if __STDC_VERSION__ >= 199901L
typedef unsigned long long int size_t;
#ifdef __NEED_SSIZE_T
typedef signed long long int ssize_t;
#endif /* __NEED_SSIZE_T */
#else
typedef unsigned long int size_t;
#ifdef __NEED_SSIZE_T
typedef signed long int ssize_t;
#endif /* __NEED_SSIZE_T */
#endif

#endif /* __SIZE_T_H */
