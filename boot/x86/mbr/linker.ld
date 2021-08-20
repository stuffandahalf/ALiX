ENTRY(_start)
/*OUTPUT_FORMAT(binary)*/
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

