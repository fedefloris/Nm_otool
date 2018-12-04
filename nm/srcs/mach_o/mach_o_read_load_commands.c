#include "nm_otool.h"
#include "nm.h"

t_sym				*mach_o_read_load_commands(t_nm_otool *nm_otool,
		t_lc *lc, t_section **sections, int number_of_commands)
{
	t_sym			*symtab;

	symtab = NULL;
	while (number_of_commands--)
	{
		if (!STRUCT_IS_SAFE(lc))
			return (NULL);
		if (!symtab && lc->cmd == LC_SYMTAB)
		{
			if (!SET(symtab, lc) || !STRUCT_IS_SAFE(symtab))
				return (NULL);
		}
		if (lc->cmd == LC_SEGMENT)//ENSURE variable load command sizes are within binary.
			if (!mach_o_32_get_sections(nm_otool, sections,
					(struct segment_command *)lc, false))//DOES THIS NEED TO BE COMMON OR UNIQUE?
				return (NULL);
		if (lc->cmd == LC_SEGMENT_64)//ENSURE variable load command sizes are within binary.
			if (!mach_o_64_get_sections(nm_otool, sections,
					(struct segment_command_64 *)lc, false))//DOES THIS NEED TO BE COMMON OR UNIQUE?
				return (NULL);
		if (lc->cmdsize <= sizeof(*lc))
			return (NULL);
		if (!SET(lc, lc + lc->cmdsize))
			return (NULL);
	}
	return (symtab);
}
