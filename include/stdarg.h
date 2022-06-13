#ifndef __STDARG_H
#define __STDARG_H 1

typedef void *va_list;

#define va_start(ap, argN) { ap = &argN; }
#define va_arg(ap, type) \
	*(type *)(ap + sizeof(type) + sizeof(type) % 4); \
	ap += sizeof(type) + sizeof(type) % 4
#define va_end(ap) ap = (void *)0

#endif /* __STDARG_H */
