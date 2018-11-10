#include "nm_otool.h"
#include "otool.h"
#include "ft_printf.h"

int     text_segment_64(struct load_command *lcmd, t_file *file)
{
    struct segment_command_64   *segment;
	struct section_64			*section;
	void						*sect_ptr;
    uint32_t                    i;

    segment = (struct segment_command_64 *)lcmd;
	sect_ptr = (void *)(segment + 1);
    i = 0;
    while (i < segment->nsects)
    {
		section = (struct section_64 *)sect_ptr;
		if (ft_strcmp(segment->segname, SEG_TEXT) == 0
				&& ft_strcmp(section->sectname, SECT_TEXT) == 0)
		{
			ft_printf("segment: %s , section: %s\n", segment->segname, section->sectname);
		}
		sect_ptr += sizeof(struct section_64);
        i++;
    }
    return (SUCCESS);
}

int     macho_64(t_file *file)
{
    struct mach_header_64       *header;
    struct load_command         *lcmd;
    uint32_t                    i;

    header = (struct mach_header_64 *)file->map;
	filetype_64(header);
    lcmd = (void *)file->map + sizeof(*header);
    i = 0;
    while (i < header->ncmds)
    {
        if (lcmd->cmd == LC_SEGMENT_64)
        {
            text_segment_64(lcmd, file);
        }
        lcmd = (void *)lcmd + lcmd->cmdsize;
        i++;
    }
    return (SUCCESS);
}