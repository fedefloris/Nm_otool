#include "nm_otool.h"
#include "otool.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int	check_file_stat(char *filename, struct stat *buf)
{
	if (buf->st_mode & S_IFREG)
		ft_printf("regular file\n");
	else if (buf->st_mode & S_IFDIR)
		ft_printf("'%s': Is a directory\n", filename);
	return (SUCCESS);
}

int load_filename(char *filename, t_file *file)
{
    if (!filename)
        return (FAILURE);
    file->name = (char *)malloc(sizeof(char) * (ft_strlen(filename) + 1));
    if (!file->name)
        return (FAILURE);
    ft_strcpy(file->name, filename);
    return (SUCCESS);
}

int load_file(char *filename, t_file *file)
{
    int			fd;
    struct stat	buf;

    if (!load_filename(filename, file))
        return (FAILURE);
    if ((fd = open(filename, O_RDONLY)) < 0)
        return (FAILURE);
    if (fstat(fd, &buf) < 0)
    {
        close(fd);
        return (FAILURE);
    }
    file->size = buf.st_size;
    if ((file->memory = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))\
            == MAP_FAILED)
    {
        close(fd);
        return (FAILURE);
    }
    return (SUCCESS);
}
