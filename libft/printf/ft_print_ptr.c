/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:04:21 by abouclie          #+#    #+#             */
/*   Updated: 2025/02/20 11:04:36 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_print_ptr(unsigned long long n)
{
	int		length;
	char	*nb;

	nb = 0;
	length = 0;
	if (!n)
	{
		ft_putstr(("(nil)"));
		return (5);
	}
	else if (n == 0)
		length += write(1, "0", 1);
	else
	{
		length = 2;
		write(1, "0x", 2);
		nb = ft_itoa_ptr(n, "0123456789abcdef");
		length += ft_printstr(nb);
	}
	free(nb);
	return (length);
}
