/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_str.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:24:09 by arocca            #+#    #+#             */
/*   Updated: 2025/07/07 09:13:01 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	in_str(char c, char *str, bool verif_alnum)
{
	if (verif_alnum && ft_isalnum(c))
		return (true);
	while (*str)
	{
		if (c == *str)
			return (true);
		str++;
	}
	return (false);
}
