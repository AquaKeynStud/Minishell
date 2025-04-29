/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:27:01 by arocca            #+#    #+#             */
/*   Updated: 2025/04/27 12:54:34 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

int	handle_string(va_list *args, ssize_t *total_len, int err)
{
	char	*arg;

	arg = (char *)va_arg(*args, char *);
	if (!arg)
	{
		ft_putstr_fd("(null)", 1);
		*total_len += 1;
	}
	else
	{
		if (err)
			return (1);
		ft_putstr_fd(arg, 1);
		*total_len += 1;
	}
	return (0);
}

int	handle_char(va_list *args, ssize_t *total_len, int err)
{
	char	arg;

	arg = (int)va_arg(*args, int);
	if (!arg)
	{
		write(1, &arg, 1);
		*total_len += 1;
		return (0);
	}
	if (err)
		return (1);
	ft_putchar_fd(arg, 1);
	*total_len += 1;
	return (0);
}

int	handle_nbr(va_list *args, ssize_t *total_len, int isInt, int err)
{
	unsigned int	arg;

	if (isInt)
	{
		arg = (int)va_arg(*args, int);
		if (err && arg)
			return (1);
		ft_putnbr_pt(arg, 1, total_len, NULL);
	}
	else
	{
		arg = (unsigned int)va_arg(*args, unsigned int);
		if (err && arg)
			return (1);
		ft_putunbr_fd(arg, 1, total_len, NULL);
	}
	return (0);
}

int	handle_address(va_list *args, ssize_t *total_len, int err)
{
	void	*arg;

	arg = (void *)va_arg(*args, void *);
	if (!arg)
	{
		write(1, "(nil)", 5);
		*total_len += 5;
		return (0);
	}
	if (err)
		return (1);
	ft_print_memory_fd(1, arg, total_len);
	return (0);
}

int	handle_hexa(va_list *args, ssize_t *total_len, int isLower, int err)
{
	unsigned int	arg;

	arg = (unsigned int)va_arg(*args, unsigned int);
	if (err && arg)
		return (1);
	if (isLower)
		convert_to_fd(1, arg, "0123456789abcdef", total_len);
	else
		convert_to_fd(1, arg, "0123456789ABCDEF", total_len);
	return (0);
}
