#ifndef ALIX_LOG_H
#define ALIX_LOG_H 1

#if __STDC_VERSION__ >= 199901L
#define klogf(fmt, ...) _klogf("%s:%ld " fmt, __FILE__, __LINE__, __VA_ARGS__)
#else
#define klogf _klogf
#endif

int _klogf(const char *fmt, ...);
int klogld(long int ld, unsigned int base);
int kloglu(unsigned long int lu, unsigned int base);
int klogc(char c);
int klogs(const char *s);

void _klog_flush(void);

#endif /* ALIX_LOG_H */
