/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_get_type.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffloris <ffloris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 11:26:40 by ffloris           #+#    #+#             */
/*   Updated: 2019/02/08 11:26:41 by ffloris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm.h"

static char	get_type(t_elf_symbols_info *info,
	unsigned char st_bind)
{
	unsigned char st_type;

	st_type = ELF32_ST_TYPE(info->st_info);
	if (info->st_shndx == SHN_ABS)
		return (st_bind == STB_LOCAL ? 'a' : 'A');
	if (info->st_shndx == SHN_COMMON)
		return ('C');
	if (st_bind == STB_WEAK && st_type == STT_OBJECT)
		return ((info->st_shndx == SHN_UNDEF) ? 'v' : 'V');
	if (st_bind == STB_WEAK)
		return ((info->st_shndx == SHN_UNDEF) ? 'w' : 'W');
	if (info->st_shndx == SHN_UNDEF)
		return ('U');
	if (st_bind == STB_GNU_UNIQUE)
		return ('u');
	return ('?');
}

char		elf_get_type(t_elf_symbols_info *info)
{
	unsigned char	st_bind;
	char			type;

	st_bind = ELF32_ST_BIND(info->st_info);
	if ((type = get_type(info, st_bind)) != '?')
		return (type);
	if ((type = elf_get_type_from_section(info)) == '?')
		type = elf_get_type_from_flags(info);
	return (st_bind == STB_LOCAL ? type : ft_toupper(type));
}
