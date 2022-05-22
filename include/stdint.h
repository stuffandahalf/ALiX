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
#define INT8_C(c) (c)
#define UINT8_C(c) (c##U)
#endif /* INT8_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT8_FMTS_DEFINED)
#define INT8_FMTS_DEFINED 1
/* fprintf signed types */
#define PRId8 "%hhd"
#define PRIi8 "%hhi"
#define PRIdLEAST8 "%hhd"
#define PRIiLEAST8 "%hhi"
#define PRIdFAST8 "%hhd"
#define PRIiFAST8 "%hhi"
/* fprintf unsigned types */
#define PRIo8 "%hho"
#define PRIu8 "%hhu"
#define PRIx8 "%hhx"
#define PRIX8 "%hhX"
/* fscanf signed types */
#define SCNd8 "%hhd"
#define SCNi8 "%hhi"
#define SCNdLEAST8 "%hhd"
#define SCNiLEAST8 "%hhi"
#define SCNdFAST8 "%hhd"
#define SCNiFAST8 "%hhd"
/* fscanf unsigned types */
#define SCNo8 "%hho"
#define SCNu8 "%hhu"
#define SCNx8 "%hhx"
#define SCNoLEAST8 "%hho"
#define SCNuLEAST8 "%hhu"
#define SCNxLEAST8 "%hhx"
#define SCNoFAST8 "%hho"
#define SCNuFAST8 "%hhu"
#define SCNxFAST8 "%hhx"
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
#define INT16_C(c) (c)
#define UINT16_C(c) (c##U)
#endif /* INT16_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT16_FMTS_DEFINED)
#define INT16_FMTS_DEFINED 1
/* fprintf signed types */
#define PRId16 "%hd"
#define PRIi16 "%hi"
#define PRIdLEAST16 "%hd"
#define PRIiLEAST16 "%hi"
#define PRIdFAST16 "%hd"
#define PRIiFAST16 "%hi"
/* fprintf unsigned types */
#define PRIo16 "%ho"
#define PRIu16 "%hu"
#define PRIx16 "%hx"
#define PRIX16 "%hX"
#define PRIoLEAST16 "%ho"
#define PRIuLEAST16 "%hu"
#define PRIxLEAST16 "%hx"
#define PRIXLEAST16 "%hX"
#define PRIoFAST16 "%ho"
#define PRIuFAST16 "%hu"
#define PRIxFAST16 "%hx"
#define PRIXFAST16 "%hX"
/* fscanf signed types */
#define SCNd16 "%hd"
#define SCNi16 "%hi"
#define SCNdLEAST16 "%hd"
#define SCNiLEAST16 "%hi"
#define SCNdFAST16 "%hd"
#define SCNiFAST16 "%hi"
/* fscanf unsigned types */
#define SCNo16 "%ho"
#define SCNu16 "%hu"
#define SCNx16 "%hx"
#define SCNoLEAST16 "%ho"
#define SCNuLEAST16 "%hu"
#define SCNxLEAST16 "%hx"
#define SCNoFAST16 "%ho"
#define SCNuFAST16 "%hu"
#define SCNxFAST16 "%hx"
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
#define INT32_C(c) (c)
#define UINT32_C(c) (c##U)
#endif /* INT32_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT32_FMTS_DEFINED)
#define INT32_FMTS_DEFINED 1
/* fprintf signed types */
#define PRId32 "%d"
#define PRIi32 "%i"
#define PRIdLEAST32 "%d"
#define PRIiLEAST32 "%i"
#define PRIdFAST32 "%d"
#define PRIiFAST32 "%i"
/* fprintf unsigned types */
#define PRIo32 "%o"
#define PRIu32 "%u"
#define PRIx32 "%x"
#define PRIX32 "%X"
#define PRIoLEAST32 "%o"
#define PRIuLEAST32 "%u"
#define PRIxLEAST32 "%x"
#define PRIXLEAST32 "%X"
#define PRIoFAST32 "%o"
#define PRIuFAST32 "%u"
#define PRIxFAST32 "%x"
#define PRIXFAST32 "%X"
/* fscanf signed types */
#define SCNd32 "%d"
#define SCNi32 "%i"
#define SCNdLEAST32 "%d"
#define SCNiLEAST32 "%i"
#define SCNdFAST32 "%d"
#define SCNiFAST32 "%i"
/* fscanf unsigned types */
#define SCNo32 "%o"
#define SCNu32 "%u"
#define SCNx32 "%x"
#define SCNoLEAST32 "%o"
#define SCNuLEAST32 "%u"
#define SCNxLEAST32 "%x"
#define SCNoFAST32 "%o"
#define SCNuFAST32 "%u"
#define SCNxFAST32 "%x"
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
#define INT32_C(c) (c##L)
#define UINT32_C(c) (c##UL)
#endif /* INT32_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT32_FMTS_DEFINED)
#define INT32_FMTS_DEFINED 1
/* fprintf signed types */
#define PRId32 "%ld"
#define PRIi32 "%li"
#define PRIdLEAST32 "%ld"
#define PRIiLEAST32 "%li"
#define PRIdFAST32 "%ld"
#define PRIiFAST32 "%li"
/* fprintf unsigned types */
#define PRIo32 "%lo"
#define PRIu32 "%lu"
#define PRIx32 "%lx"
#define PRIX32 "%lX"
#define PRIoLEAST32 "%lo"
#define PRIuLEAST32 "%lu"
#define PRIxLEAST32 "%lx"
#define PRIXLEAST32 "%lX"
#define PRIoFAST32 "%lo"
#define PRIuFAST32 "%lu"
#define PRIxFAST32 "%lx"
#define PRIXFAST32 "%lX"
/* fscanf signed types */
#define SCNd32 "%ld"
#define SCNi32 "%li"
#define SCNdLEAST32 "%ld"
#define SCNiLEAST32 "%li"
#define SCNdFAST32 "%ld"
#define SCNiFAST32 "%li"
/* fscanf unsigned types */
#define SCNo32 "%lo"
#define SCNu32 "%lu"
#define SCNx32 "%lx"
#define SCNoLEAST32 "%lo"
#define SCNuLEAST32 "%lu"
#define SCNxLEAST32 "%lx"
#define SCNoFAST32 "%lo"
#define SCNuFAST32 "%lu"
#define SCNxFAST32 "%lx"
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
#define INT32_C(c) (c)
#define UINT32_C(c) (c##U)
#endif /* INT32_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT32_FMTS_DEFINED)
/* fprintf signed types */
#define PRIdLEAST32 "%d"
#define PRIiLEAST32 "%i"
#define PRIdFAST32 "%d"
#define PRIiFAST32 "%i"
/* fprintf unsigned types */
#define PRIoLEAST32 "%o"
#define PRIuLEAST32 "%u"
#define PRIxLEAST32 "%x"
#define PRIXLEAST32 "%X"
#define PRIoFAST32 "%o"
#define PRIuFAST32 "%u"
#define PRIxFAST32 "%x"
#define PRIXFAST32 "%X"
/* fscanf signed types */
#define SCNdLEAST32 "%d"
#define SCNiLEAST32 "%i"
#define SCNdFAST32 "%d"
#define SCNiFAST32 "%i"
/* fscanf unsigned types */
#define SCNoLEAST32 "%o"
#define SCNuLEAST32 "%u"
#define SCNxLEAST32 "%x"
#define SCNoFAST32 "%o"
#define SCNuFAST32 "%u"
#define SCNxFAST32 "%x"
#endif /* __INTTYPES_H */
#endif


#if defined(__LP64__) || defined(__ILP64__)
#ifndef INT64_TYPES_DEFINED
#define INT64_TYPES_DEFINED 1
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
#define INT64_C(c) (c##L)
#define UINT64_C(c) (c##UL)
#endif /* INT64_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT64_FMTS_DEFINED)
#define INT64_TYPES_DEFINED 1
/* fprintf signed types */
#define PRId64 "%ld"
#define PRIi64 "%li"
#define PRIdLEAST64 "%ld"
#define PRIiLEAST64 "%li"
#define PRIdFAST64 "%ld"
#define PRIiFAST64 "%li"
/* fprintf unsigned types */
#define PRIo64 "%lo"
#define PRIu64 "%lu"
#define PRIx64 "%lx"
#define PRIX64 "%lX"
#define PRIoLEAST64 "%lo"
#define PRIuLEAST64 "%lu"
#define PRIxLEAST64 "%lx"
#define PRIXLEAST64 "%lX"
#define PRIoFAST64 "%lo"
#define PRIuFAST64 "%lu"
#define PRIxFAST64 "%lx"
#define PRIXFAST64 "%lX"
/* fscanf signed types */
#define SCNd64 "%ld"
#define SCNi64 "%li"
#define SCNdLEAST64 "%ld"
#define SCNiLEAST64 "%li"
#define SCNdFAST64 "%ld"
#define SCNiFAST64 "%li"
/* fscanf unsigned types */
#define SCNo64 "%lo"
#define SCNu64 "%lu"
#define SCNx64 "%lx"
#define SCNoLEAST64 "%lo"
#define SCNuLEAST64 "%lu"
#define SCNxLEAST64 "%lx"
#define SCNoFAST64 "%lo"
#define SCNuFAST64 "%lu"
#define SCNxFAST64 "%lx"
#endif /* __INTTYPES_H */
#elif __STDC_VERSION__ >= 199901L
#ifndef INT64_TYPES_DEFINED
#define INT64_TYPES_DEFINED 1
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
#define INT64_C(c) (c##LL)
#define UINT64_C(c) (c##ULL)
#endif /* INT64_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INT64_FMTS_DEFINED)
#define INT64_TYPES_DEFINED 1/* fprintf signed types */
#define PRId64 "%lld"
#define PRIi64 "%lli"
#define PRIdLEAST64 "%lld"
#define PRIiLEAST64 "%lli"
#define PRIdFAST64 "%lld"
#define PRIiFAST64 "%lli"
/* fprintf unsigned types */
#define PRIo64 "%llo"
#define PRIu64 "%llu"
#define PRIx64 "%llx"
#define PRIX64 "%llX"
#define PRIoLEAST64 "%llo"
#define PRIuLEAST64 "%llu"
#define PRIxLEAST64 "%llx"
#define PRIXLEAST64 "%llX"
#define PRIoFAST64 "%llo"
#define PRIuFAST64 "%llu"
#define PRIxFAST64 "%llx"
#define PRIXFAST64 "%llX"
/* fscanf signed types */
#define SCNd64 "%lld"
#define SCNi64 "%lli"
#define SCNdLEAST64 "%lld"
#define SCNiLEAST64 "%lli"
#define SCNdFAST64 "%lld"
#define SCNiFAST64 "%lli"
/* fscanf unsigned types */
#define SCNo64 "%llo"
#define SCNu64 "%llu"
#define SCNx64 "%llx"
#define SCNoLEAST64 "%llo"
#define SCNuLEAST64 "%llu"
#define SCNxLEAST64 "%llx"
#define SCNoFAST64 "%llo"
#define SCNuFAST64 "%llu"
#define SCNxFAST64 "%llx"
#endif /* __INTTYPES_H */
#endif

#if defined(__LLP64__) && __STDC_VERSION__ >= 199901L
#ifndef INTPTR_TYPES_DEFINED
#define INTPTR_TYPES_DEFINED 1
typedef signed long long int intptr_t;
typedef unsigned long long int uintptr_t;
#define INTPTR_MIN INTN_MIN(64)
#define INTPTR_MAX INTN_MAX(64)
#define UINTPTR_MAX UINTN_MAX(64)
#endif /* INTPTR_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INTPTR_FMTS_DEFINED)
/* fprintf signed types */
#define PRIdPTR "%lld"
#define PRIiPTR "%lli"
/* fprintf unsigned types */
#define PRIoPTR "%llo"
#define PRIuPTR "%llu"
#define PRIxPTR "%llx"
#define PRIXPTR "%llX"
/* fscanf signed types */
#define SCNdPTR "%lld"
#define SCNiPTR "%lli"
/* fscanf unsigned types */
#define SCNoPTR "%llo"
#define SCNuPTR "%llu"
#define SCNxPTR "%llx"
#endif /* __INTTYPES_H */
#elif defined(__LP64__) || defined(__ILP64__)
#ifndef INTPTR_TYPES_DEFINED
#define INTPTR_TYPES_DEFINED 1
typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;
#define INTPTR_MIN INTN_MIN(64)
#define INTPTR_MAX INTN_MAX(64)
#define UINTPTR_MAX UINTN_MAX(64)
#endif /* INTPTR_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INTPTR_FMTS_DEFINED)
/* fprintf signed types */
#define PRIdPTR "%ld"
#define PRIiPTR "%li"
/* fprintf unsigned types */
#define PRIoPTR "%lo"
#define PRIuPTR "%lu"
#define PRIxPTR "%lx"
#define PRIXPTR "%lX"
/* fscanf signed types */
#define SCNdPTR "%ld"
#define SCNiPTR "%li"
/* fscanf unsigned types */
#define SCNoPTR "%lo"
#define SCNuPTR "%lu"
#define SCNxPTR "%lx"
#endif /* __INTTYPES_H */
#elif defined(__ILP32__) || defined(__LP32__)
#ifndef INTPTR_TYPES_DEFINED
#define INTPTR_TYPES_DEFINED 1
typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;
#define INTPTR_MIN INTN_MIN(32)
#define INTPTR_MAX INTN_MAX(32)
#define UINTPTR_MAX UINTN_MAX(32)
#endif /* INTPTR_TYPES_DEFINED */
#if defined(__INTTYPES_H) && !defined(INTPTR_FMTS_DEFINED)
#define INTPTR_FMTS_DEFINED
/* fprintf signed types */
#define PRIdPTR "%ld"
#define PRIdPTR "%li"
/* fprintf unsigned types */
#define PRIoPTR "%lo"
#define PRIuPTR "%lu"
#define PRIxPTR "%lx"
#define PRIXPTR "%lX"
/* fscanf signed types */
#define SCNdPTR "%ld"
#define SCNiPTR "%li"
/* fscanf unsigned types */
#define SCNoPTR "%lo"
#define SCNuPTR "%lu"
#define SCNxPTR "%lx"
#endif /* __INTTYPES_H */
#endif

#if defined(INT64_TYPES_DEFINED)
#if !defined(INTMAX_TYPES_DEFINED)
#define INTMAX_TYPES_DEFINED 1
typedef int64_t intmax_t;
typedef uint64_t uintmax_t;
#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX
#endif /* INTMAX_TYPES_DEFINED */
#if defined(__INTTYPES_H) && defined(INT64_FMTS_DEFINED)
/* fprintf signed types */
#define PRIdMAX PRId64
#define PRIiMAX PRIi64
/* fprintf unsigned types */
#define PRIoMAX PRIo64
#define PRIuMAX PRIu64
#define PRIxMAX PRIx64
#define PRIXMAX PRIX64
/* fscanf signed types */
#define SCNdMAX SCNd64
#define SCNiMAX SCNi64
/* fscanf unsigned types */
#define SCNoMAX SCNo64
#define SCNuMAX SCNu64
#define SCNxMAX SCNx64
#endif /* INTTYPES_H */
#elif defined(INT32_TYPES_DEFINED)
#if !defined(INTMAX_TYPES_DEFINED)
#define INTMAX_TYPES_DEFINED 1
typedef int32_t intmax_t;
typedef uint32_t uintmax_t;
#define INTMAX_MIN INT32_MIN
#define INTMAX_MAX INT32_MAX
#define UINTMAX_MAX UINT32_MAX
#endif /* INTMAX_TYPES_DEFINED */
#if defined(__INTTYPES_H) && defined(INT32_FMTS_DEFINED)
/* fprintf signed types */
#define PRIdMAX PRId32
#define PRIiMAX PRIi32
/* fprintf unsigned types */
#define PRIoMAX PRIo32
#define PRIuMAX PRIu32
#define PRIxMAX PRIx32
#define PRIXMAX PRIX32
/* fscanf signed types */
#define SCNdMAX SCNd32
#define SCNiMAX SCNi32
/* fscanf unsigned types */
#define SCNoMAX SCNo32
#define SCNuMAX SCNu32
#define SCNxMAX SCNx32
#endif /* __INTTYPES_H */
#else
#if !defined(INTMAX_TYPES_DEFINED)
#define INTMAX_TYPES_DEFINED 1
typedef int16_t intmax_t;
typedef uint16_t uintmax_t;
#define INTMAX_MIN INT16_MIN
#define INTMAX_MAX INT16_MAX
#define UINTMAX_MAX UINT16_MAX
#endif /* INTMAX_TYPES_DEFINED */
#if defined(__INTTYPES_H) && defined(INT16_FMTS_DEFINED)
/* fprintf signed types */
#define PRIdMAX PRId16
#define PRIiMAX PRIi16
/* fprintf unsigned types */
#define PRIoMAX PRIo16
#define PRIuMAX PRIu16
#define PRIxMAX PRIx16
#define PRIXMAX PRIX16
/* fscanf signed types */
#define SCNdMAX SCNd16
#define SCNiMAX SCNi16
/* fscanf unsigned types */
#define SCNoMAX SCNo16
#define SCNuMAX SCNu16
#define SCNxMAX SCNx16
#endif /* __INTTYPES_H */
#endif
