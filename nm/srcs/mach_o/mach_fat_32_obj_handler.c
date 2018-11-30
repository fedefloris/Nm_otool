#include "nm_otool.h"
#include "nm.h"

#include <mach-o/fat.h>
#include <mach/machine.h>
#include "ft_printf.h"
#include "nm_otool.h"

#include <mach-o/fat.h>

void	 swap_64(unsigned char word)
{
	unsigned char	tmp;
	unsigned char	new_word;
	int				i;

	i = 0;
	while (i < 4)
	{
		tmp = word & 3;
		new_word = new_word << 2;
		new_word |= tmp;
		word = word >> 2;
		i++;
	}
	//ft_printf("------->value = %.2x\n", y);
}

uint32_t    swap_endian(uint32_t value)
{
	uint32_t    result;
	uint32_t    byte1;
	uint32_t    byte2;
	uint32_t    byte3;
	uint32_t    byte4;

	byte1 = (value & 0xFF) << 24;
	byte2 = (value & 0xFF00) << 8;
	byte3 = (value & 0xFF0000) >> 8;
	byte4 = (value & 0xFF000000) >> 24;
	result = byte1 | byte2 | byte3 | byte4;
	return (result);
}

static bool			process_fat(t_nm_otool *nm_otool, struct fat_arch *arch, uint32_t nfat_arch, bool (*macho)(t_nm_otool *))
{
	bool			status;
	t_file			file_data;

	file_data = nm_otool->file;
	status = false;
	while (nfat_arch--)
	{
		ft_bzero(&nm_otool->file, sizeof(nm_otool->file));
		nm_otool->file.name = file_data.name;
		nm_otool->file.size = arch->size;//Check this is good size.
		nm_otool->file.memory = (void *)file_data.memory + swap_endian(arch->offset);
		nm_otool->file.end_of_file = file_data.memory + file_data.size - 1;
		nm_otool->file.endianness = file_data.endianness;
		if (macho(nm_otool))
			status = true;
		arch = (struct fat_arch *)((void *)arch + sizeof(struct fat_arch));
	}
	nm_otool->file = file_data;
	return (status);
}

static bool			handle_fat(t_nm_otool *nm_otool, struct fat_arch *arch, uint32_t nfat_arch)
{
	if (!get_safe_address(nm_otool, (char *)arch + sizeof(*arch)))
		return (false);
	if (swap_endian(arch->cputype) == CPU_TYPE_X86_64)
		return (process_fat(nm_otool, arch, nfat_arch, &mach_o_64_obj_handler));
	else if (swap_endian(arch->cputype) == CPU_TYPE_I386)
		return (process_fat(nm_otool, arch, nfat_arch, &mach_o_32_obj_handler));
	return (false);
}

bool				mach_fat_32_obj_handler(t_nm_otool *nm_otool)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	uint32_t			nfat_arch;

	if (!(header = (struct fat_header *)get_safe_address(
			nm_otool, (char *)nm_otool->file.memory)))
		return (false);
	if (!get_safe_address(nm_otool, (char *)header + sizeof(*header)))
		return (false);
	nfat_arch = swap_endian(header->nfat_arch);
	if (!(arch = (struct fat_arch *)get_safe_address(
			nm_otool, (char *)nm_otool->file.memory + sizeof(*header))))
		return (false);
	handle_fat(nm_otool, arch, nfat_arch);
	return (true);
}
