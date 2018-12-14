#include "nm_otool.h"

static void	config_file(t_nm_otool *nm_otool, char *file_name)
{
	ft_bzero(&nm_otool->file, sizeof(nm_otool->file));
	nm_otool->file.name = file_name;
}

int			list_obj_symbols(t_nm_otool *nm_otool,
	char *file_name, t_obj_handler obj_handler)
{
	config_file(nm_otool, file_name);
	if (!set_file(nm_otool))
		return (EXIT_FAILURE);
	if (obj_handler && !obj_handler(nm_otool))
		return (EXIT_FAILURE);
	if (!unset_file(nm_otool))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
