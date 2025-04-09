/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 11:02:34 by arocca            #+#    #+#             */
/*   Updated: 2025/04/09 11:50:10 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	int		size;
	char	*dest;
	int		i;

	size = ft_strlen(s);
	i = 0;
	dest = (char *)malloc(sizeof(char) * size + 1);
	if (!dest)
		return (dest);
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t	i;
	char	*old_ptr;
	char	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	old_ptr = (char *)ptr;
	i = 0;
	while (i < old_size && i < new_size)
	{
		((char *)new_ptr)[i] = old_ptr[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
