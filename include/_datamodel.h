#if !defined(__LP64__) && \
		!defined(__ILP64__) && \
		!defined(__LLP64__) && \
		!defined(__ILP32__) && \
		!defined(__LP32__)
#error Unsupported data model
#endif