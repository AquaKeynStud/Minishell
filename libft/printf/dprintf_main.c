/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprintf_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:01:41 by arocca            #+#    #+#             */
/*   Updated: 2025/04/27 15:51:28 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

static int	dprintf_parser(int fd, char c, va_list *args, ssize_t *total_len)
{
	if (c == 'd' || c == 'i' || c == 'u')
		return (handle_nbr_fd(fd, args, total_len, (c != 'u')) == 0);
	if (c == 'c')
		return (handle_char_fd(fd, args, total_len) == 0);
	if (c == 's')
		return (handle_string_fd(fd, args, total_len) == 0);
	if (c == 'p')
		return (handle_address_fd(fd, args, total_len) == 0);
	if (c == 'x' || c == 'X')
		return (handle_hexa_fd(fd, args, total_len, (c == 'x')) == 0);
	if (c == '%')
	{
		ft_putchar_fd('%', fd);
		*total_len += 1;
	}
	return (1);
}

static int	parse_args_fd(int fd, char *s, va_list *args, ssize_t *total_len)
{
	if (check_conv(s[0]))
		return (dprintf_parser(fd, s[0], args, total_len));
	return (*total_len += write(fd, "%", 1));
}

static size_t	format_update_fd(int fd, char **s, ssize_t *len, va_list *args)
{
	size_t	i;

	i = 0;
	while ((*s)[i])
	{
		if ((*s)[i] == '%')
		{
			write(1, *s, i);
			*s += i + parse_args_fd(fd, *s + i + 1, args, len) + 1;
			if (*len == -1)
				return (1);
			*len += i;
			i = 0;
		}
		else
			i++;
	}
	return (i);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	args;
	size_t	i;
	ssize_t	len;
	char	*s;

	i = 0;
	len = 0;
	if (!format || fd <= 0)
		return (-1);
	va_start(args, format);
	s = (char *)format;
	if (s[0] == '%' && (!s[1] || (s[1] == '\n' && !s[2])))
		return (-1);
	else if (s[0] == '%' && !check_conv(s[1]) && auth_char(s[1]) == -1 && !s[2])
		return (-1);
	i += format_update_fd(fd, &s, &len, &args);
	if (len == -1)
		return (-1);
	ft_putstr_fd(s, fd);
	len += i;
	va_end(args);
	return (len);
}
