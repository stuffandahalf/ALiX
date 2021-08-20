ENTRY(_start)
SECTIONS
{
	. = 0x7c00;
	.text :
	{
		*(.text)
		*(.data)
		*(.rodata)
	}

	/DISCARD/ :
	{
		*(.note.gnu.property)
	}
}

