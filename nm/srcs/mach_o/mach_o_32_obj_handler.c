#include "nm_otool.h"
#include "nm.h"

static bool			mach_o_32_read_symbols(t_nm_otool *nm_otool,
		struct nlist_64 *array, t_section *sections,
		t_symbol **symbols, t_sym *symtab)
{
	uint32_t		i;
	char			*str;
	char			*stringtable;

	i = 0;
	if (!(stringtable = (char *)get_safe_address(nm_otool,
			(char *)nm_otool->file.memory + symtab->stroff)))
		return (false);
	while (i < symtab->nsyms)
	{
		if (!get_safe_address(nm_otool, (char *)&array[i] + sizeof(array[i])))
			return (false);
		if (!(str = (char *)get_safe_address(nm_otool,
				(char *)stringtable + array[i].n_un.n_strx)))
			return (false);
		if (!string_is_safe(nm_otool, (char *)str))
			return (false);
		if ((array[i].n_type & N_STAB) == 0)
			if (!(add_symbol(symbols, array[i].n_value,
					get_type(array[i].n_type, array[i].n_value,
					array[i].n_sect, sections), str)))
				return (false);
		i++;
	}
	return (true);
}

static bool			mach_o_32_get_symbols(t_nm_otool *nm_otool,
		t_sym *symtab, t_section *sections)
{
	struct nlist_64	*array;
	t_symbol		*symbols;

	symbols = NULL;
	if (!(array = (struct nlist_64 *)get_safe_address(nm_otool,
			(char *)nm_otool->file.memory + symtab->symoff)))
		return (free_symbols(symbols));
	if (!(mach_o_32_read_symbols(nm_otool, array,
			sections, &symbols, symtab)))
		return (free_symbols(symbols));
	display_symbols(nm_otool, symbols);
	free_sections(sections);
	free_symbols(symbols);
	return (true);
}

static int			mach_o_32_get_first_load_command(t_nm_otool *nm_otool,
		t_lc **lc)
{
	struct mach_header		*header;

	if (!(header = (struct mach_header *)get_safe_address(
			nm_otool, (char *)nm_otool->file.memory)))
		return (-1);
	if (!(*lc = (t_lc *)get_safe_address(
			nm_otool, (char *)nm_otool->file.memory + sizeof(*header))))
		return (-1);
	if (!get_safe_address(nm_otool, (char *)header + sizeof(*header)))
		return (-1);
	return ((int)header->ncmds);
}

static t_sym		*mach_o_32_read_load_commands(t_nm_otool *nm_otool,
		t_lc *lc, t_section **sections, int number_of_commands)
{
	t_sym			*symtab;

	symtab = NULL;
	while (number_of_commands--)
	{
		if (!get_safe_address(nm_otool, (char *)lc + sizeof(*lc)))
			return (NULL);
		if (!symtab && lc->cmd == LC_SYMTAB)
		{
			if (!(symtab = (t_sym *)get_safe_address(nm_otool, (char *)lc))
				|| !get_safe_address(nm_otool, (char *)lc + sizeof(*symtab)))
				return (NULL);
		}
		if (lc->cmd == LC_SEGMENT_64)
			if (!mach_o_64_get_sections(nm_otool, sections,
					(struct segment_command_64 *)lc))
				return (NULL);
		if (lc->cmdsize <= sizeof(*lc))
			return (NULL);
		if (!(lc = (t_lc *)get_safe_address(nm_otool,
				(char *)lc + lc->cmdsize)))
			return (NULL);
	}
	return (symtab);
}

bool				mach_o_32_obj_handler(t_nm_otool *nm_otool)
{
	int						number_of_commands;
	t_lc					*lc;
	t_sym					*symtab;
	t_section				*sections;

	sections = NULL;
	if ((number_of_commands =
			mach_o_32_get_first_load_command(nm_otool, &lc)) < 0)
		return (free_sections(sections));
	if ((symtab = mach_o_32_read_load_commands(nm_otool, lc,
			&sections, number_of_commands)))
		return (mach_o_32_get_symbols(nm_otool, symtab, sections));
	return (true);//Is this good or bad? TRUE/FALSE?
}

