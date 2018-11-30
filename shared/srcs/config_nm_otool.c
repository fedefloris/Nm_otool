#include "nm_otool.h"

bool				config_nm_otool(t_nm_otool *nm_otool, char **argv, char **env)
{
	ft_bzero(nm_otool, sizeof(*nm_otool));
	if (!options(&argv, NM_OPTIONS, &nm_otool->options))
		return (false);
	nm_otool->argv = argv;
	nm_otool->env = env;
	nm_otool->print_file_name = (*argv && *(argv + 1)) ? true : false;
	return (true);
}
