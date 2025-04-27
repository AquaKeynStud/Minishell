/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:27:07 by arocca            #+#    #+#             */
/*   Updated: 2025/04/27 13:05:43 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"

void	ft_putnbr_pt(int n, int fd, ssize_t *total_len, int (*f)[8])
{
	long	nb;
	char	digit;

	if (f && !(*f)[6] && (*f)[2] && n == 0)
	{
		*total_len += write(fd, " ", 1);
		return ;
	}
	nb = (long)n;
	if ((!f && n < 0))
		*total_len += write(fd, "-", 1);
	if (nb < 0)
		nb = -nb;
	if (nb <= 9)
	{
		digit = '0' + nb;
		*total_len += write(fd, &digit, 1);
	}
	else
	{
		ft_putnbr_pt(nb / 10, fd, total_len, NULL);
		digit = '0' + (nb % 10);
		*total_len += write(fd, &digit, 1);
	}
	return ;
}

void	ft_putunbr_fd(unsigned int n, int fd, ssize_t *total_len, int (*f)[8])
{
	unsigned long	nb;
	char			digit;

	if (f && !(*f)[6] && (*f)[2] && n == 0)
	{
		*total_len += write(fd, " ", 1);
		return ;
	}
	nb = (unsigned long)n;
	if (nb <= 9)
	{
		digit = '0' + nb;
		*total_len += write(fd, &digit, 1);
	}
	else
	{
		ft_putunbr_fd(nb / 10, fd, total_len, NULL);
		digit = '0' + (nb % 10);
		*total_len += write(fd, &digit, 1);
	}
	return ;
}

void	convert_to_fd(int fd, unsigned long nbr, char *base, ssize_t *total_len)
{
	unsigned long	base_len;

	base_len = ft_strlen(base);
	if (nbr < base_len)
		*total_len += write(fd, &base[nbr % base_len], 1);
	else
	{
		convert_to_fd(fd, nbr / base_len, base, total_len);
		*total_len += write(fd, &base[nbr % base_len], 1);
	}
	return ;
}

void	ft_print_memory_fd(int fd, void *addr, ssize_t *total_len)
{
	unsigned long	a;

	a = (unsigned long)addr;
	*total_len += write(fd, "0x", 2);
	convert_to_fd(fd, a, "0123456789abcdef", total_len);
	return ;
}
