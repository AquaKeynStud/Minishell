/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:27:17 by arocca            #+#    #+#             */
/*   Updated: 2025/01/15 14:27:17 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <limits.h>
#include <stdint.h>

int	auth_char(char c)
{
	char	*conv;
	int		i;

	i = 0;
	conv = "-0.+ #123456789";
	while (conv[i])
	{
		if (conv[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

char	check_conv(char c)
{
	char	*types;

	types = "cspdiuxX%";
	while (*types)
	{
		if (*types == c)
			return (*types);
		types++;
	}
	return (0);
}
