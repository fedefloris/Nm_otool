#include "nm_otool.h"
#include "nm.h"

static bool	parse_section_headers(t_nm_otool *nm_otool,
	Elf32_Ehdr *header, Elf32_Shdr *section_headers,
	t_elf_symbols_info *info)
{
	uint32_t			link_index;
	Elf32_Off			sh_offset;
	uint16_t			e_shnum;

	e_shnum = SWAP_ENDIAN(header->e_shnum);
	while (info->index < e_shnum)
	{
		if (!STRUCT_IS_SAFE(&section_headers[info->index]))
			return (ERROR_LOG("not enough space for an other section header"));
		link_index = SWAP_ENDIAN(section_headers[info->index].sh_link);
		if (!STRUCT_IS_SAFE(&section_headers[link_index]))
			return (ERROR_LOG("sh_link outside the section headers array"));
		sh_offset = SWAP_ENDIAN(section_headers[link_index].sh_offset);
		if (!SET(info->str_section, (char*)header + sh_offset))
			return (ERROR_LOG("not enough space for the string table"));
		if (!elf_parse_section_header_32(nm_otool, section_headers, info))
			return (false);
		info->index++;
	}
	return (true);
}

static bool	set_header_str_section(t_nm_otool *nm_otool,
	Elf32_Ehdr *header, Elf32_Shdr *section_headers,
	t_elf_symbols_info *info)
{
	Elf32_Off			sh_offset;

	if (!STRUCT_IS_SAFE(&section_headers[header->e_shstrndx]))
		return (ERROR_LOG("e_shstrndx outside the section headers array"));
	sh_offset = SWAP_ENDIAN(section_headers[header->e_shstrndx].sh_offset);
	if (!SET(info->header_str_section, (char*)header + sh_offset))
		return (ERROR_LOG("not enough space for the string table"));
	return (true);
}

bool		elf_parse_section_headers_32(t_nm_otool *nm_otool,
	Elf32_Ehdr *header)
{
	Elf32_Shdr			*section_headers;
	t_elf_symbols_info	info;

	if (!SET(section_headers, (char*)header + SWAP_ENDIAN(header->e_shoff)))
		return (ERROR_LOG("not enough space for the first section header"));
	ft_bzero(&info, sizeof(info));
	if (header->e_shstrndx != SHN_UNDEF
		&& !set_header_str_section(nm_otool, header, section_headers, &info))
		return (false);
	return (parse_section_headers(nm_otool, header, section_headers, &info));
}
