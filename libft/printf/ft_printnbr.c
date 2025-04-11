/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:10:22 by abouclie          #+#    #+#             */
/*   Updated: 2025/02/20 11:10:35 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_printnbr(long int n, char *base)
{
	char	*nb;
	int		len;

	len = 0;
	nb = ft_itoa_base(n, base);
	len = ft_printstr(nb);
	free(nb);
	return (len);
}
