#include "nm_otool.h"
#include "nm.h"

static bool	is_initialized_data_section(t_elf_symbols_info *info)
{
	return (info->sh_type == SHT_PROGBITS
		|| info->sh_type == SHT_DYNAMIC);
}

char		elf_get_type_from_flags(t_elf_symbols_info *info)
{
	if ((info->sh_flags & SHF_WRITE) != SHF_WRITE)
	{
		if (info->sh_type == SHT_GROUP)
			return ('n');
		return ('r');
	}
	if (is_initialized_data_section(info))
		return ('d');
	return ('?');
}
