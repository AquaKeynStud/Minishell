/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_ptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:00:18 by abouclie          #+#    #+#             */
/*   Updated: 2025/02/20 17:25:27 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_itoa_ptr(uintptr_t n, char *base)
{
	int			i;
	int			size;
	char		*str;

	i = 0;
	size = ft_ptrlen(n);
	str = malloc((size + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[size] = '\0';
	while (size-- > i)
	{
		str[size] = base[n % 16];
		n /= 16;
	}
	return (str);
}
