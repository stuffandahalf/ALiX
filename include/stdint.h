#ifndef __STDINT_H
#define __STDINT_H 1

#if !defined(__LP64__) && \
	!defined(__ILP64__) && \
	!defined(__LLP64__) && \
	!defined(__ILP32__) && \
	!defined(__LP32__)
#error Unsupported data model
#endif

#define INTN_MIN(N) (-(1 << (N - 1)))
#define INTN_MAX(N) ((1 << (N - 1)) - 1)
#define UINTN_MAX(N) ((1 << N) - 1)

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


#if defined(__LP64__) || defined(__LLP64__) || defined(__ILP32__)
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
#elif defined(__LP32__)
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
#elif defined(__ILP64__)
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


#if defined(__LLP64__) && __STDC_VERSION__ >= 199901L
typedef signed long long int intptr_t;
typedef unsigned long long int uintptr_t;
#define INTPTR_MIN INTN_MIN(64)
#define INTPTR_MAX INTN_MAX(64)
#define UINTPTR_MAX UINTN_MAX(64)
#elif defined(__LP64__) || defined(__ILP64__)
typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;
#define INTPTR_MIN INTN_MIN(64)
#define INTPTR_MAX INTN_MAX(64)
#define UINTPTR_MAX UINTN_MAX(64)
#elif defined(__ILP32__) || defined(__LP32__)
typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;
#define INTPTR_MIN INTN_MIN(32)
#define INTPTR_MAX INTN_MAX(32)
#define UINTPTR_MAX UINTN_MAX(32)
#endif

#endif /* __STDINT_H */
