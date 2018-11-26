#include "nm_otool.h"
#include "nm.h"

static char			get_type_64(uint8_t n_type, u_int64_t n_value, u_int8_t n_sect, t_section *sections)
{
	char			type;

	type = '0';
	type = ((n_type & N_TYPE) == N_UNDF && n_value) ? 'C' : type;
	type = ((n_type & N_TYPE) == N_UNDF && !n_value) ? 'U' : type;
	type = ((n_type & N_TYPE) == N_ABS && !n_value) ? 'A' : type;
	type = ((n_type & N_TYPE) == N_PBUD && !n_value) ? 'U' : type;
	if ((n_type & N_TYPE) == N_SECT)
	{
		while (sections)
		{
			if (n_sect == sections->sec_number)
				break ;
			sections = sections->next;
		}
		if (!sections)
			type = 'S';
		else
		{
			type = (!ft_strcmp(sections->name, SECT_BSS)) ? 'B' : type;
			type = (!ft_strcmp(sections->name, SECT_DATA)) ? 'D' : type;
			type = (!ft_strcmp(sections->name, SECT_TEXT)) ? 'T' : type;
		}
	}
	type = ((n_type & N_TYPE) == N_INDR && !n_value) ? 'I' : type;
	type = ((n_type & N_STAB) != 0) ? 'Z' : type;
	type += ((n_type & N_EXT) == 0 && type != '0') ? 32 : 0;
	return (type);
}

static bool			free_sections(t_section *sections)
{
	t_section		*tmp;

	while (sections)
	{
		tmp = sections;
		sections = sections->next;
		free(tmp);
	}
	return (false);
}

static bool			get_symbols_64(t_nm_otool *nm_otool, struct symtab_command *symtab, t_section *sections)
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
		if (!get_safe_address(nm_otool, (char *)array + sizeof(*array)))
			return (false);
		if (!(str = (char *)get_safe_address(nm_otool, (char *)stringtable + array[i].n_un.n_strx)))
			return (false);
		if (!string_is_safe(nm_otool, (char *)str))
			return (false);
		(array[i].n_value) ? ft_printf("%-17.16x", array[i].n_value) : ft_printf("%17s", "");
		ft_printf("%c ", get_type_64(array[i].n_type, array[i].n_value, array[i].n_sect, sections));
		ft_printf("%s\n", str);
		i++;
	}
	free_sections(sections);
	return (true);
}

static bool			get_sections_64(t_nm_otool *nm_otool, t_section **sections, struct segment_command_64 *segment)//maybe does not have to be 64 only.
{
	uint32_t				i;
	static unsigned char	sec_number = 1;
	struct section_64		*sec;
	t_section				*new;

	i = 0;
	if (!(sec = (struct section_64 *)get_safe_address(nm_otool, (char *)segment + sizeof(*segment))))
		return (false);
	while (i++ < segment->nsects)
	{
		ft_printf("SEG: [%s]\n", segment->segname);
		if (!get_safe_address(nm_otool, (char *)sec + sizeof(*sec)))
			return (false);
		if (!string_is_safe(nm_otool, (char *)sec->sectname))
			return (false);
		if (!ft_strcmp(sec->sectname, SECT_DATA)
			|| !ft_strcmp(sec->sectname, SECT_BSS)
			|| !ft_strcmp(sec->sectname, SECT_TEXT))
		{
			if (!(new = (t_section *)ft_memalloc(sizeof(t_section))))
				return (false);//FREE
			new->name = sec->sectname;
			new->sec_number = sec_number;
			if (!*sections)
				*sections = new;
			else
			{
				new->next = *sections;
				*sections = new;
			}
		}
		if (!(sec = (struct section_64 *)get_safe_address(nm_otool, (char *)sec + sizeof(*sec))))
			return (false);
		sec_number++;
	}	
	return (true);
}

bool				mach_o_64_obj_handler(t_nm_otool *nm_otool)
{
	uint32_t				i;
	struct mach_header_64	*header;
	t_lc					*lc;
	struct symtab_command	*symtab;
	t_section				*sections;

	i = 0;
	symtab = NULL;
	sections = NULL;
	if (!(header = (struct mach_header_64 *)get_safe_address(nm_otool, (char *)nm_otool->file.memory)))
		return (free_sections(sections));
	if (!(lc = (t_lc *)get_safe_address(nm_otool, (char *)nm_otool->file.memory + sizeof(*header))))
		return (free_sections(sections));
	if (!get_safe_address(nm_otool, (char *)header + sizeof(*header)))
		return (free_sections(sections));
	while (i++ < header->ncmds)
	{
		if (!get_safe_address(nm_otool, (char *)lc + sizeof(*lc)))
			return (free_sections(sections));
		if (!symtab && lc->cmd == LC_SYMTAB)
		{
			if (!(symtab = (struct symtab_command *)get_safe_address(nm_otool, (char *)lc))
				|| !get_safe_address(nm_otool, (char *)lc + sizeof(*symtab)))
				return (free_sections(sections));
		}
		if (lc->cmd == LC_SEGMENT_64)
			if (!get_sections_64(nm_otool, &sections, (struct segment_command_64 *)lc))
				return (free_sections(sections));
		if (lc->cmdsize <= sizeof(*lc))
			return (free_sections(sections));
		if (!(lc = (t_lc *)get_safe_address(nm_otool, (char *)lc + lc->cmdsize)))
			return (free_sections(sections));
	}
	if (symtab)
		return (get_symbols_64(nm_otool, symtab, sections));
	return (true);//Is this good or bad? TRUE/FALSE?
}
