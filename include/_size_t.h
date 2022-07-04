#ifndef __SIZE_T
#define __SIZE_T 1
#if __STDC_VERSION__ >= 199901L
typedef unsigned long long int size_t;
#else
typedef unsigned long int size_t;
#endif /* C99 */
#endif /* __SIZE_T */

#ifdef __NEED_SSIZE_T
#ifndef __SSIZE_T
#define __SSIZE_T
#if __STDC_VERSION__ >= 199901L
typedef long long int ssize_t;
#else
typedef long int ssize_t;
#endif /* C99 */
#endif /* __SSIZE_T */
#endif /* __NEED_SSIZE_T */
