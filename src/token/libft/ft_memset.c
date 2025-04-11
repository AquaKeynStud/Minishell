/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:31:14 by abouclie          #+#    #+#             */
/*   Updated: 2024/11/18 16:08:38 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*dest;
	size_t			i;

	dest = s;
	i = 0;
	while (i++ < n)
	{
		*dest++ = c;
	}
	return (s);
}
