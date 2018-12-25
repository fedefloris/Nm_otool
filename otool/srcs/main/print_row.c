#include "nm_otool.h"
#include "otool.h"

static char	ft_calculate_char(int mod, char c)
{
	char	return_char;

	if (mod > 36)
		return (0);
	return_char = '0';
	while (mod--)
	{
		return_char++;
		if (return_char == ':')
			return_char = c;
	}
	return (return_char);
}

static int	ft_get_len(uintmax_t num, uintmax_t base)
{
	int		len;

	len = 0;
	while (num)
	{
		num /= base;
		len++;
	}
	return (len);
}

//Get rid of this and use with your own ft_itoa_base (the one in our dev libft does not work)
static char	*ft_itoa_base_tmp(uintmax_t num, uintmax_t base, char c)
{
	uintmax_t	sum;
	int			mod;
	int			len;
	char		*str;

	if (num == 0)
	{
		if (!(str = ft_strnew(1)))
			return (NULL);
		*str = '0';
		return (str);
	}
	sum = num;
	len = ft_get_len(num, base);
	if (!(str = ft_strnew(len)))
		return (NULL);
	while (sum)
	{
		mod = sum % base;
		sum /= base;
		str[(len--) - 1] = ft_calculate_char(mod, c);
	}
	return (str);
}

void		print_row(unsigned char *byte)
{
	unsigned char	row[(BYTES_PER_ROW * 3) + 1];
	char			*number;
	int				current_byte;
	int				position_on_row;

	current_byte = 0;
	position_on_row = 0;
	number = NULL;
	while (position_on_row < BYTES_PER_ROW)
	{
		if ((number = ft_itoa_base_tmp(*byte, 16, 'a')))//Get rid of this and use with your own ft_itoa_base (the one in our dev libft does not work)
		{
			if (ft_strlen(number) == 1)
				row[current_byte++] = '0';
			ft_strcpy((char *)(row + current_byte), number);
			current_byte += ft_strlen(number);
		}
		else
			ft_strcpy((char *)(row + current_byte), "00");

		row[current_byte++] = ' ';
		position_on_row++;
		byte++;
		ft_strdel(&number);
	}
	row[current_byte] = '\0';
	ft_printf("%s", row);
}
