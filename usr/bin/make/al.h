#ifndef AL_H
#define AL_H 1

#define ARRAYLIST(T) \
	struct { \
		T *a; \
		size_t c; \
		size_t sz; \
	}


#define AL_APPEND(T, al, el) { \
	if (al.c == al.sz) { \
		al.sz += 3; \
		al.a = realloc(al.a, sizeof(T) * al.sz); \
	} \
	if (al.a) { \
		al.a[al.c] = el; \
		al.c++; \
	} \
}

#endif /* AL_H */

