#ifndef __STDINT_H
#define __STDINT_H 1

#include "_datamodel.h"

#endif

#define INTN_MIN(N) (-(1 << (N - 1)))
#define INTN_MAX(N) ((1 << (N - 1)) - 1)
#define UINTN_MAX(N) ((1 << N) - 1)

#ifndef INT8_TYPES_DEFINED
#define INT8_TYPES_DEFINED 1
/* all supported data models use 8-bit chars */
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;
#define INT8_MIN INTN_MIN(8)
#define INT8_MAX INTN_MAX(8)
#define UINT8_MAX UINTN_MAX(8)
#define INT_LEAST8_MIN INTN_MIN(8)
#define INT_LEAST8_MAX INTN_MAX(8)
#define UINT_LEAST8_MAX UINTN_MAX(8)
#define INT_FAST8_MIN INTN_MIN(8)
#define INT_FAST8_MAX INTN_MAX(8)
#define UINT_FAST8_MAX UINTN_MAX(8)
#endif /* INT8_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT8_FMTS_DEFINED)
#define INT8_FMTS_DEFINED 1
#define PRId8 "%d"
#define PRIi8 "%i"
#define PRIdLEAST8 "%d"
#define PRIiLEAST8 "%i"
#define PRIdFAST8 "%d"
#define PRIiFAST8 "%i"
#endif /* __INTTYPES_H */

#ifndef INT16_TYPES_DEFINED
#define INT16_TYPES_DEFINED 1
/* all supported data models use 16-bit shorts */
typedef signed short int int16_t;
typedef unsigned short int uint16_t;
typedef signed short int int_least16_t;
typedef unsigned short int uint_least16_t;
typedef signed short int int_fast16_t;
typedef unsigned short int uint_fast16_t;
#define INT16_MIN INTN_MIN(16)
#define INT16_MAX INTN_MAX(16)
#define UINT16_MAX UINTN_MAX(16)
#define INT_LEAST16_MIN INTN_MIN(16)
#define INT_LEAST16_MAX INTN_MAX(16)
#define UINT_LEAST16_MAX UINTN_MAX(16)
#define INT_FAST16_MIN INTN_MIN(16)
#define INT_FAST16_MAX INTN_MAX(16)
#define UINT_FAST16_MAX UINTN_MAX(16)
#endif /* INT16_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT16_FMTS_DEFINED)
#define INT16_FMTS_DEFINED 1
#define PRId16 "%hd"
#define PRIi16 "%hi"
#define PRIdLEAST16 "%hd"
#define PRIiLEAST16 "%hi"
#define PRIdFAST16 "%hd"
#define PRIiFAST16 "%hi"
#endif /* __INTTYPES_H */


#if defined(__LP64__) || defined(__LLP64__) || defined(__ILP32__)
#ifndef INT32_TYPES_DEFINED
#define INT32_TYPES_DEFINED 1
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed int int_least32_t;
typedef unsigned int uint_least32_t;
typedef signed int int_fast32_t;
typedef unsigned int uint_fast32_t;
#define INT32_MIN INTN_MIN(32)
#define INT32_MAX INTN_MAX(32)
#define UINT32_MAX UINTN_MAX(32)
#define INT_LEAST32_MIN INTN_MIN(32)
#define INT_LEAST32_MAX INTN_MAX(32)
#define UINT_LEAST32_MAX UINTN_MAX(32)
#define INT_FAST32_MIN INTN_MIN(32)
#define INT_FAST32_MAX INTN_MAX(32)
#define UINT_FAST32_MAX UINTN_MAX(32)
#endif /* INT32_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT32_FMTS_DEFINED)
#define INT32_FMTS_DEFINED 1
#define PRId32 "%d"
#define PRIi32 "%i"
#define PRIdLEAST32 "%d"
#define PRIiLEAST32 "%i"
#define PRIdFAST32	"%d"
#define PRIiFAST32	"%i"
#endif /* __INTTYPES_H */
#elif defined(__LP32__)
#ifndef INT32_TYPES_DEFINED
#define INT32_TYPES_DEFINED 1
typedef signed long int int32_t;
typedef unsigned long int uint32_t;
typedef signed long int int_least32_t;
typedef unsigned long int uint_least32_t;
typedef signed long int int_fast32_t;
typedef unsigned long int uint_fast32_t;
#define INT32_MIN INTN_MIN(32)
#define INT32_MAX INTN_MAX(32)
#define UINT32_MAX UINTN_MAX(32)
#define INT_LEAST32_MIN INTN_MIN(32)
#define INT_LEAST32_MAX INTN_MAX(32)
#define UINT_LEAST32_MAX UINTN_MAX(32)
#define INT_FAST32_MIN INTN_MIN(32)
#define INT_FAST32_MAX INTN_MAX(32)
#define UINT_FAST32_MAX UINTN_MAX(32)
#endif /* INT32_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT32_FMTS_DEFINED)
#define INT32_FMTS_DEFINED 1
/* TODO: Populate formats for LP32 data model */
#endif /* __INTTYPES_H */
#elif defined(__ILP64__)
#ifndef INT32_TYPES_DEFINED
#define INT32_TYPES_DEFINED 1
/* no exact width integers for 32-bit */
typedef signed int int_least32_t;
typedef unsigned int uint_least32_t;
typedef signed int int_fast32_t;
typedef unsigned int uint_fast32_t;
#define INT_LEAST32_MIN INTN_MIN(64)
#define INT_LEAST32_MAX INTN_MAX(64)
#define UINT_LEAST32_MAX UINTN_MAX(64)
#define INT_FAST32_MIN INTN_MIN(64)
#define INT_FAST32_MAX INTN_MAX(64)
#define UINT_FAST32_MAX UINTN_MAX(64)
#endif /* INT32_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT32_FMTS_DEFINED)
/* TODO: Populate formats for ILP64 data model */
#endif /* __INTTYPES_H */
#endif


#if defined(__LP64__) || defined(__ILP64__)
typedef signed long int int64_t;
typedef unsigned long int uint64_t;
typedef signed long int int_least64_t;
typedef unsigned long int uint_least64_t;
typedef signed long int int_fast64_t;
typedef unsigned long int uint_fast64_t;
#define INT64_MIN INTN_MIN(64)
#define INT64_MAX INTN_MAX(64)
#define UINT64_MAX UINTN_MAX(64)
#define INT_LEAST64_MIN INTN_MIN(64)
#define INT_LEAST64_MAX INTN_MAX(64)
#define UINT_LEAST64_MAX UINTN_MAX(64)
#define INT_FAST64_MIN INTN_MIN(64)
#define INT_FAST64_MAX INTN_MAX(64)
#define UINT_FAST64_MAX UINTN_MAX(64)
#elif __STDC_VERSION__ >= 199901L
typedef signed long long int int64_t;
typedef unsigned long long int uin64_t;
typedef signed long long int int_least64_t;
typedef unsigned long long int uint_least64_t;
typedef signed long long int int_fast64_t;
typedef unsigned long long int uint_fast64_t;
#define INT64_MIN INTN_MIN(64)
#define INT64_MAX INTN_MAX(64)
#define UINT64_MAX UINTN_MAX(64)
#define INT_LEAST64_MIN INTN_MIN(64)
#define INT_LEAST64_MAX INTN_MAX(64)
#define UINT_LEAST64_MAX UINTN_MAX(64)
#define INT_FAST64_MIN INTN_MIN(64)
#define INT_FAST64_MAX INTN_MAX(64)
#define UINT_FAST64_MAX UINTN_MAX(64)
#endif

/* TODO: Add support for intmax_t */

#if defined(__LLP64__) && __STDC_VERSION__ >= 199901L
typedef signed long long int intptr_t;
typedef unsigned long long int uintptr_t;
#define INTPTR_MIN INTN_MIN(64)
#define INTPTR_MAX INTN_MAX(64)
#define UINTPTR_MAX UINTN_MAX(64)
#ifdef __INTTYPES_H
#define PRIdPTR "%lld"
#define PRIiPTR	"%lli"
#endif /* __INTTYPES_H */
#elif defined(__LP64__) || defined(__ILP64__)
typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;
#define INTPTR_MIN INTN_MIN(64)
#define INTPTR_MAX INTN_MAX(64)
#define UINTPTR_MAX UINTN_MAX(64)
#ifdef __INTTYPES_H
#define PRIdPTR "%lld"
#define PRIiPTR "%lli"
#endif /* __INTTYPES_H */
#elif defined(__ILP32__) || defined(__LP32__)
typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;
#define INTPTR_MIN INTN_MIN(32)
#define INTPTR_MAX INTN_MAX(32)
#define UINTPTR_MAX UINTN_MAX(32)
#ifdef __INTTYPES_H
#define PRIdPTR "%lld"
#define PRIdPTR "%lli"
#endif /* __INTTYPES_H */
#endif

#endif /* __STDINT_H */
