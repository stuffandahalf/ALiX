ENTRY(_start)
SECTIONS
{
	. = 0x7c00;

	top = .;

	.text :
	{
		bootld.o(.text)
		bootld.o(.data)
		bootld.o(.rodata)
		bootld.o(.bss)

		*(.text)
		*(.data)
		*(.rodata)
		*(.bss)
	}

	bottom = .;

	/DISCARD/ :
	{
		*(.note.gnu.property)
	}
}

