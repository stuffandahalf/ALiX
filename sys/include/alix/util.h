#ifndef ALIX_UTIL_H
#define ALIX_UTIL_H

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define LOC() klogs(__FILE__ ":"); kloglu(__LINE__, 10);

#ifndef NDEBUG
#define NOT_IMPLEMENTED() \
	LOC(); \
	klogs(" NOT YET IMPLEMENTED\n");
#else
#define NOT_IMPLEMENTED()
#endif

#define PANIC() LOC(); klogs("PANIC\n"); _klog_flush(); while (1);

#define LIST_INCSZ 3
#define LIST(T) \
	struct { \
		T *list; \
		size_t size; \
		size_t length; \
	}

#define LIST_INIT { NULL, 0, 0 }
#define LIST_INITIALIZE(l) { \
	(l)->list = NULL; \
	(l)->size = 0; \
	(l)->length = 0; \
}
#define LIST_FREE(l) { \
	kfree((l)->list); \
	LIST_INITIALIZE((l)); \
}

/* void LIST_INSERT(TYPE, LIST(TYPE) *, int, TYPE *) */
#define LIST_INSERT(T, l, i, e) { \
	if ((i) >= (l)->size) { \
		(l)->size = i + 1; \
		(l)->list = krealloc((l)->list, sizeof(T) * (l)->size); \
	} \
	if ((l)->list) { \
		if (!(l)->list[(i)]) { \
			(l)->length++; \
		} \
		(l)->list[(i)] = (e); \
	} \
}

#define LIST_APPEND(T, l, e) { \
	if ((l)->size == (l)->length) { \
		(l)->size += LIST_INCSZ; \
		(l)->list = krealloc((l)->list, sizeof(T) * (l)->size); \
	} \
	(l)->list[(l)->length++] = (e); \
}

// #define LIST_DEL(T, )

#endif /* ALIX_UTIL_H */