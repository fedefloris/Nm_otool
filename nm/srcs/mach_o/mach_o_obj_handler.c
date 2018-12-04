#include "nm_otool.h"
#include "nm.h"

bool		mach_o_obj_handler(t_nm_otool *nm_otool)
{
	mach_o_32_get_sections(NULL, NULL, NULL, true);
	mach_o_64_get_sections(NULL, NULL, NULL, true);
	if (nm_otool->file.format == MACH_O_32)
		return (mach_o_32_obj_handler(nm_otool));
	if (nm_otool->file.format == MACH_O_64)
		return (mach_o_64_obj_handler(nm_otool));
	if (nm_otool->file.format == MACH_O_FAT_32)
		return (mach_fat_32_obj_handler(nm_otool));
	if (nm_otool->file.format == MACH_O_FAT_64)
		return (mach_fat_64_obj_handler(nm_otool));
	if (nm_otool->file.format == MACH_O_AR	CHIVE)
		return (mach_o_archive(nm_otool));
	return (false);
}
