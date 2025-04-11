/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 10:48:10 by abouclie          #+#    #+#             */
/*   Updated: 2025/02/20 11:01:07 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_itoa_base(long int n, char *base)
{
	int			i;
	int			size;
	int			bsize;
	char		*str;

	i = 0;
	bsize = ft_strlen(base);
	size = ft_intlen(n, bsize);
	str = malloc((size + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	if (n < 0 && bsize == 10)
	{
		str[i] = '-';
		n *= -1;
		i++;
	}
	str[size] = '\0';
	while (size-- > i)
	{
		str[size] = base[n % bsize];
		n /= bsize;
	}
	return (str);
}
