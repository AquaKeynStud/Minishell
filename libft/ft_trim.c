/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:17:33 by arocca            #+#    #+#             */
/*   Updated: 2025/04/25 14:42:51 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

static bool	is_in_set(char c, char *set)
{
	while (*set)
	{
		if (c == *set++)
			return (true);
	}
	return (false);
}

void	ft_trim(char **str, char *set)
{
	int		len;
	int		start;
	char	*trimmed;

	start = 0;
	if (!str || !(*str) || !set)
		return ;
	while (is_in_set((*str)[start], set))
		start++;
	len = ft_strlen(*str);
	while ((len > start) && is_in_set((*str)[len - 1], set))
		len--;
	len -= start;
	if (len <= 0)
		return ;
	trimmed = malloc(sizeof(char) * (len + 1));
	if (!trimmed)
		return ;
	trimmed[len] = '\0';
	while (len--)
		trimmed[len] = (*str)[start + len];
	free(*str);
	*str = trimmed;
}
