#include "nm_otool.h"
#include "nm.h"

bool		elf_64_parse_section_headers(t_nm_otool *nm_otool,
	Elf64_Ehdr *header)
{
	Elf64_Shdr					*section_headers;
	uint32_t						link_index;
	Elf64_Off						sh_offset;
	t_elf_symbols_info	info;

	if (!SET(section_headers, (char*)header + header->e_shoff))
		return (ERROR_LOG("not enough space for the first section header"));
	ft_bzero(&info, sizeof(info));
	while (info.index < header->e_shnum)
	{
		if (!STRUCT_IS_SAFE(&section_headers[info.index]))
			return (ERROR_LOG("not enough space for an other section header"));
		link_index = section_headers[info.index].sh_link;
		if (!STRUCT_IS_SAFE(&section_headers[link_index]))
			return (ERROR_LOG("sh_link index is outside the section headers array"));
		sh_offset = section_headers[link_index].sh_offset;
		if (!SET(info.str_section, (char*)header + sh_offset))
			return (ERROR_LOG("not enough space for the string table"));
		if (!elf_64_parse_section_header(nm_otool, section_headers, &info))
			return (false);
		info.index++;
	}
	return (true);
}
