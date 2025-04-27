/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:01:41 by arocca            #+#    #+#             */
/*   Updated: 2025/04/27 12:48:56 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	mandatory_parser(char c, va_list *args, ssize_t *total_len, int err)
{
	if (c == 'd' || c == 'i' || c == 'u')
		return (handle_nbr(args, total_len, (c != 'u'), err) == 0);
	if (c == 'c')
		return (handle_char(args, total_len, err) == 0);
	if (c == 's')
		return (handle_string(args, total_len, err) == 0);
	if (c == 'p')
		return (handle_address(args, total_len, err) == 0);
	if (c == 'x' || c == 'X')
		return (handle_hexa(args, total_len, (c == 'x'), err) == 0);
	if (c == '%')
	{
		ft_putchar_fd('%', 1);
		*total_len += 1;
	}
	return (1);
}

void	format_update(char **s, ssize_t	*i, ssize_t *len, va_list *args)
{
	while ((*s)[(*i)])
	{
		if ((*s)[(*i)] == '%')
		{
			write(1, *s, *i);
			*s += *i + parse_args(*s + *i + 1, args, len) + 1;
			if (*len == -1)
				return ;
			*len += *i;
			*i = 0;
		}
		else
			(*i)++;
	}
	return ;
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	ssize_t	i;
	ssize_t	len;
	char	*s;

	i = 0;
	len = 0;
	if (!format)
		return (-1);
	va_start(args, format);
	s = (char *)format;
	if (s[0] == '%' && (!s[1] || (s[1] == '\n' && !s[2])))
		return (-1);
	else if (s[0] == '%' && !check_conv(s[1])
		&& auth_char(s[1]) == -1 && !s[2])
		return (-1);
	format_update(&s, &i, &len, &args);
	if (len == -1)
		return (-1);
	ft_putstr_fd(s, 1);
	len += i;
	va_end(args);
	return (len);
}
