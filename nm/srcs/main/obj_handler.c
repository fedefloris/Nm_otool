#include "nm_otool.h"
#include "nm.h"

bool		obj_handler(t_nm_otool *nm_otool)
{
	if (IS_ELF_FORMAT(nm_otool->file.format))
		return (elf_obj_handler(nm_otool));
	return (MAC_OBJ_HANDLER(nm_otool));
}
