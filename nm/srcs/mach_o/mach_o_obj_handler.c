#include "nm_otool.h"
#include "nm.h"

bool		mach_o_obj_handler(t_nm_otool *nm_otool)
{
	mach_o_get_sections_32(NULL, NULL, NULL, true);
	mach_o_get_sections_64(NULL, NULL, NULL, true);
	if (nm_otool->file.format == MACH_O_32)
		return (mach_o_32_obj_handler(nm_otool));
	if (nm_otool->file.format == MACH_O_64)
		return (mach_o_64_obj_handler(nm_otool));
	if (nm_otool->file.format == MACH_O_FAT_32)
		return (mach_o_fat_32(nm_otool));
	if (nm_otool->file.format == MACH_O_FAT_64)
		return (mach_o_fat_64(nm_otool));
	if (nm_otool->file.format == MACH_O_ARCHIVE)
		return (mach_o_archive(nm_otool));
	return (ERROR_LOG("Unrecognized format"));
}
