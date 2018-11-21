#include "nm_otool.h"
#include "nm.h"

static bool			print_nm(t_nm_otool *nm_otool, struct symtab_command *symtab)
{
	uint32_t		i;
	char			*str;
	char			*stringtable;
	struct nlist_64	*array;

	i = 0;
	if (!(array = (struct nlist_64 *)get_safe_address(nm_otool, (char *)nm_otool->file.memory + symtab->symoff)))
		return (false);
	if (!(stringtable = (char *)get_safe_address(nm_otool, (char *)nm_otool->file.memory + symtab->stroff)))
		return (false);
	while (i < symtab->nsyms)
	{
		if (!get_safe_address(nm_otool, (char *)symtab + sizeof(symtab)))
			return (false);
		if (!(str = (char *)get_safe_address(nm_otool, (char *)stringtable + array[i].n_un.n_strx)))
			return (false);
		if (!string_is_safe(nm_otool, (char *)str))
			return (false);
		ft_printf("%16.16x ", array[i].n_value);
		ft_printf("%s\n", str);
		i++;
	}
	return (true);
}

bool				mach_o_64_obj_handler(t_nm_otool *nm_otool)
{
	int						number_of_commands;
	struct mach_header_64	*header;
	t_lc					*lc;
	struct symtab_command	*symtab;

	if (!(header = (struct mach_header_64 *)get_safe_address(nm_otool, (char *)nm_otool->file.memory)))
		return (false);
	if (!(lc = (t_lc *)get_safe_address(nm_otool, (char *)nm_otool->file.memory + sizeof(*header))))
		return (false);
	if (!get_safe_address(nm_otool, (char *)header + sizeof(*header)))
		return (false);
	else
		number_of_commands = header->ncmds;
	while (true)
	{
		if (get_safe_address(nm_otool, (char *)lc + sizeof(*lc)) && lc->cmd == LC_SYMTAB)
		{
			if (!(symtab = (struct symtab_command *)get_safe_address(nm_otool, (char *)lc))
				|| !get_safe_address(nm_otool, (char *)symtab + sizeof(*symtab)))
				return (false);
			return (print_nm(nm_otool, symtab));
			break ;
		}
		if (!(lc = (t_lc *)get_safe_address(nm_otool, (char *)lc + lc->cmdsize)))
			return (false);
		if (!number_of_commands--)
			return (false);
	}
	return (true);
}
