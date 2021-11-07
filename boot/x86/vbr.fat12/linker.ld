ENTRY(_start)
SECTIONS
{
	. = 0x7c00;

	top = .;

	.text :
	{
		vbr.fat12.o(.text)
		vbr.fat12.o(.data)
		vbr.fat12.o(.rodata)
		vbr.fat12.o(.bss)

		../read.o(.text)
		../read.o(.data)
		../read.o(.rodata)
		../read.o(.bss)
	}

	. = 0x7dfe;
	.text :
	{
		../boot_sig.o(.text)
		../boot_sig.o(.data)
		../boot_sig.o(.rodata)
		../boot_sig.o(.bss)
	}

	bottom = .;

	/DISCARD/ :
	{
		*(.note.gnu.property)
	}
}

