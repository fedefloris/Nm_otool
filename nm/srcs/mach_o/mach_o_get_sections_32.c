/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_o_get_sections_32.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffloris <ffloris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 11:27:20 by ffloris           #+#    #+#             */
/*   Updated: 2019/02/08 11:27:21 by ffloris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "nm.h"

bool				mach_o_get_sections_32(t_nm_otool *nm_otool,
		uint8_t **sections, struct segment_command *segment, bool reset)
{
	uint32_t				i;
	static unsigned char	sec_number = 1;
	struct section			*sec;

	sec_number = (reset) ? 1 : sec_number;
	if (reset)
		return (true);
	i = 0;
	if (!SET(sec, segment + sizeof(*segment)))
		return (ERROR_LOG("section beyond binary"));
	while (i++ < SWAP_ENDIAN(segment->nsects))
	{
		if (!STRUCT_IS_SAFE(sec))
			return (ERROR_LOG("section beyond binary"));
		if (!string_is_safe(nm_otool, (char *)sec->sectname))
			return (ERROR_LOG("sec->sect_name is beyond binary"));
		if (!ft_strcmp(sec->sectname, SECT_DATA)
				|| !ft_strcmp(sec->sectname, SECT_BSS)
				|| !ft_strcmp(sec->sectname, SECT_TEXT))
			sections[sec_number] = (uint8_t *)sec->sectname;
		if (!(NEXT_STRUCT(sec)))
			return (ERROR_LOG("section beyond binary"));
		sec_number++;
	}
	return (true);
}
