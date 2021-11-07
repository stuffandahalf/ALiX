ENTRY(_start)
SECTIONS
{
	. = 0x7dfe;
	.text :
	{
		../boot_sig.o(.text)
		../boot_sig.o(.data)
		../boot_sig.o(.rodata)
		../boot_sig.o(.bss)
	}

	. = 0x7c00;
	.text :
	{
		*(.text)
		*(.data)
		*(.rodata)

		../read.o(.text)
		../read.o(.data)
		../read.o(.rodata)
	}

	/DISCARD/ :
	{
		*(.note.gnu.property)
	}
}

