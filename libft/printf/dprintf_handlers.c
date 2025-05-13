/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprintf_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:27:01 by arocca            #+#    #+#             */
/*   Updated: 2025/04/27 13:12:24 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	handle_string_fd(int fd, va_list *args, ssize_t *total_len)
{
	char	*arg;

	arg = (char *)va_arg(*args, char *);
	if (!arg)
	{
		ft_putstr_fd("(null)", fd);
		*total_len += 6;
	}
	else
	{
		ft_putstr_fd(arg, fd);
		*total_len += ft_strlen(arg);
	}
	return (0);
}

int	handle_char_fd(int fd, va_list *args, ssize_t *total_len)
{
	char	arg;

	arg = (int)va_arg(*args, int);
	if (!arg)
	{
		write(fd, &arg, 1);
		*total_len += 1;
		return (0);
	}
	ft_putchar_fd(arg, fd);
	*total_len += 1;
	return (0);
}

int	handle_nbr_fd(int fd, va_list *args, ssize_t *total_len, int isInt)
{
	unsigned int	arg;

	if (isInt)
	{
		arg = (int)va_arg(*args, int);
		if (arg)
			ft_putnbr_pt(arg, fd, total_len, NULL);
	}
	else
	{
		arg = (unsigned int)va_arg(*args, unsigned int);
		if (arg)
			ft_putunbr_fd(arg, fd, total_len, NULL);
	}
	return (0);
}

int	handle_address_fd(int fd, va_list *args, ssize_t *total_len)
{
	void	*arg;

	arg = (void *)va_arg(*args, void *);
	if (!arg)
	{
		*total_len += write(fd, "(nil)", 5);
		return (0);
	}
	ft_print_memory_fd(fd, arg, total_len);
	return (0);
}

int	handle_hexa_fd(int fd, va_list *args, ssize_t *total_len, int isLower)
{
	unsigned int	arg;

	arg = (unsigned int)va_arg(*args, unsigned int);
	if (!arg)
		return (1);
	if (isLower)
		convert_to_fd(fd, arg, "0123456789abcdef", total_len);
	else
		convert_to_fd(fd, arg, "0123456789ABCDEF", total_len);
	return (0);
}
