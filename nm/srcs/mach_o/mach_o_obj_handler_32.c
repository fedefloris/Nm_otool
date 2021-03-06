/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_o_obj_handler_32.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffloris <ffloris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 11:27:29 by ffloris           #+#    #+#             */
/*   Updated: 2019/02/08 11:27:31 by ffloris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm.h"

static bool			mach_o_32_read_symbols(t_nm_otool *nm_otool,
		struct nlist *array, t_reader *reader)
{
	uint32_t		i;
	char			*str;
	char			*stringtable;

	i = 0;
	if (!SET(stringtable, nm_otool->file.memory
			+ SWAP_ENDIAN(reader->symtab->stroff)))
		return (ERROR_LOG("no stringtable found"));
	while (i < SWAP_ENDIAN(reader->symtab->nsyms))
	{
		if (!STRUCT_IS_SAFE(&array[i]))
			return (ERROR_LOG("struct nlist is not a good size"));
		if (!SET(str, stringtable + SWAP_ENDIAN(array[i].n_un.n_strx)))
			str = BAD_STRING_INDEX;
		else if (!STRING_IS_SAFE(str))
			return (ERROR_LOG("symbol name str goes beyond the binary limit"));
		if ((SWAP_ENDIAN(array[i].n_type) & N_STAB) == 0)
			if (!(add_symbol(&reader->symbols, SWAP_ENDIAN(array[i].n_value),
					mach_o_get_type(SWAP_ENDIAN(array[i].n_type),
					(uint64_t)SWAP_ENDIAN(array[i].n_value),
					SWAP_ENDIAN(array[i].n_sect), reader->sections), str)))
				return (ERROR_LOG("malloc failed: t_symbol symbol"));
		i++;
	}
	return (true);
}

static bool			mach_o_32_get_symbols(t_nm_otool *nm_otool,
		t_sym *symtab, uint8_t **sections)
{
	struct nlist	*array;
	t_reader		reader;

	reader.sections = sections;
	reader.symbols = NULL;
	reader.symtab = symtab;
	if (!SET(array, nm_otool->file.memory + SWAP_ENDIAN(reader.symtab->symoff)))
		return (free_symbols(reader.symbols));
	if (!(mach_o_32_read_symbols(nm_otool, array, &reader)))
		return (free_symbols(reader.symbols));
	sort_symbols(nm_otool, &reader.symbols);
	display_symbols(nm_otool, reader.symbols);
	free_symbols(reader.symbols);
	return (true);
}

static int			mach_o_32_get_first_load_command(t_nm_otool *nm_otool,
		t_lc **lc)
{
	struct mach_header		*header;

	if (!SET(header, nm_otool->file.memory))
		return (-1);
	if (!SET(*lc, header + sizeof(*header)))
		return (-1);
	return ((STRUCT_IS_SAFE(header)) ? (int)SWAP_ENDIAN(header->ncmds) : -1);
}

bool				mach_o_obj_handler_32(t_nm_otool *nm_otool)
{
	int					number_of_commands;
	uint8_t				*sections[256];
	t_lc				*lc;
	t_sym				*symtab;

	ft_bzero(sections, sizeof(uint8_t*) * 256);
	if ((number_of_commands =
			mach_o_32_get_first_load_command(nm_otool, &lc)) < 0)
		return (false);
	if ((symtab = mach_o_read_load_commands(nm_otool, lc,
			sections, number_of_commands)))
		return (mach_o_32_get_symbols(nm_otool, symtab, sections));
	return (true);
}
