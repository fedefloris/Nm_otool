#include "nm_otool.h"
#include "nm.h"

static void			print_nm(t_nm_otool *nm_otool, struct symtab_command *symtab)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;

	array = nm_otool->file.memory + symtab->symoff;
	stringtable = nm_otool->file.memory + symtab->stroff;
	i = 0;
	while (i < symtab->nsyms)
	{
		ft_printf("%s\n",stringtable + array[i].n_un.n_strx);
		i++;
	}
}

bool				mach_o_64_obj_handler(t_nm_otool *nm_otool)
{
	int						number_of_commands;
	struct mach_header_64	*header;
	struct load_command		*load_command;
	struct symtab_command	*symtab;

	header = (struct mach_header_64 *)nm_otool->file.memory;
	number_of_commands = header->ncmds;
	load_command = nm_otool->file.memory + sizeof(*header);
	int i = 0;//DELETE
	while (number_of_commands--)
	{
		if (load_command->cmd == LC_SYMTAB)
		{
			symtab = (struct symtab_command *) load_command;
			print_nm(nm_otool, symtab);
			break ;
		}
		load_command = (void *)load_command + load_command->cmdsize;
		//load_command += 0;//delete
		ft_printf("%d\n", i++);
		ft_printf("MEM: %p\nSIZ: %x\nEND: %p\nADD: %p\n",
			nm_otool->file.memory, nm_otool->file.size, nm_otool->file.end_of_file, load_command);
		if (!is_safe_address(nm_otool, (void *)load_command))
		{
			ft_printf("LOL\n\n\n");
		}
	}
	return (true);
}
