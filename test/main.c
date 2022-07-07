//#include <stdio.h>

extern void test(void);
int main(int argc, char **argv);

void _start(void) {
	main(0, (void *)0);
}

int main(int argc, char **argv)
{
	test();
	return 0;
}

