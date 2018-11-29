#include "nm_otool.h"
#include <mach-o/loader.h>
#include <stdint.h>

static bool	set_mach_o(t_nm_otool *nm_otool, uint32_t magic_number)
{
	if (magic_number == MH_MAGIC || magic_number == MH_CIGAM)
		nm_otool->file.format = MACH_O_32_FORMAT;
	else if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
	{
		nm_otool->file.format = MACH_O_64_FORMAT;
		if (nm_otool->file.size < (long)sizeof(struct mach_header_64))
		{
			ERROR_LOG("Bad size");
			return (false);
		}
	}
	else
		return (false);
	nm_otool->file.endianness = LITTLE_ENDIAN_TYPE;
	if (magic_number == MH_CIGAM || magic_number == MH_CIGAM_64)
		nm_otool->file.endianness = BIG_ENDIAN_TYPE;
	return (true);
}

static bool	set_fat(t_nm_otool *nm_otool, uint32_t magic_number)
{
	if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		nm_otool->file.format = MACH_O_FAT_32;
	else if (magic_number == FAT_MAGIC_64 || magic_number == FAT_CIGAM_64)
		nm_otool->file.format = MACH_O_FAT_64;
	else
		return (false);
	nm_otool->file.endianness = LITTLE_ENDIAN_TYPE;
	if (magic_number == FAT_CIGAM || magic_number == FAT_CIGAM_64)
		nm_otool->file.endianness = BIG_ENDIAN_TYPE;
	return (true);
}

static bool	set_format(t_nm_otool *nm_otool)
{
	uint32_t		magic_number;

	magic_number = *(uint32_t *)nm_otool->file.memory;
	if (!set_mach_o(nm_otool, magic_number) && !set_fat(nm_otool, magic_number))
		return (false);
	return (true);
}

bool		set_mach_o_info(t_nm_otool *nm_otool)
{
	if (nm_otool->file.size < (long)sizeof(struct mach_header))
		ERROR_LOG("Bad size");
	else if (!set_format(nm_otool))
		ERROR_LOG("Bad magic number");
	else
		return (true);
	return (false);
}
