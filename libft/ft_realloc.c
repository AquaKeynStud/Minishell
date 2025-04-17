/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:44:56 by arocca            #+#    #+#             */
/*   Updated: 2025/04/12 17:49:43 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t	i;
	char	*new;

	i = 0;
	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}
	new = malloc(new_size);
	if (!new)
		return (NULL);
	while (ptr && i < old_size && i < new_size)
	{
		new[i] = ((char *)ptr)[i];
		i++;
	}
	free(ptr);
	return (new);
}
