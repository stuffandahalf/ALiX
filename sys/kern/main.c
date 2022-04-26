unsigned char *const framebuffer = (unsigned char *)0xb8000;

void
main(void)
{
	const char *str = "Hello kernel world!";
	const char *c;
	int i = 0;
	/*for (c = str; *c != '\0'; c++) {
		framebuffer[i++] = *c;
		framebuffer[i++] = 0xf0;
	}*/
}
