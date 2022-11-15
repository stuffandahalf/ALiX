#ifndef ALIX_LOG_H
#define ALIX_LOG_H 1

int klogld(long int ld, unsigned int base);
int kloglu(unsigned long int lu, unsigned int base);
int klogc(char c);
int klogs(const char *s);

void _klog_flush(void);

#endif /* ALIX_LOG_H */
