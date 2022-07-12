ENTRY(_start)
SECTIONS
{
	. = 0x0600;
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

