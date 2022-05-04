ENTRY(_start)
SECTIONS
{
	. = 1M;
	kernel_bottom = .;
	.text : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	.data : ALIGN(4K)
	{
		*(.data)
	}

	.rodata : ALIGN(4K)
	{
		*(.rodata)
	}

	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
	kernel_top = .;
}

