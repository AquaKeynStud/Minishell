/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/05/20 11:11:18 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include "parsing.h"

t_ctx	*set_ctx(t_ctx *ctx)
{
	static t_ctx	*context;

	if (ctx)
		context = ctx;
	else if (!context)
		return (NULL);
	return (context);
}

void	*s_malloc(size_t size)
{
	void	*alloc;

	if (!size)
		exit(1);
	alloc = malloc(size);
	if (!alloc)
	{
		perror("malloc");
		exit(1);
	}
	return (alloc);
}

void	*s_calloc(size_t nmemb, size_t size)
{
	void	*alloc;

	if (!nmemb || !size || nmemb > SIZE_MAX / size)
		return (NULL);
	alloc = ft_calloc(nmemb, size);
	if (!alloc)
	{
		perror("ft_calloc");
		exit(1);
	}
	return (alloc);
}

void	*s_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*alloc;

	if (!new_size)
		return (NULL);
	alloc = ft_realloc(ptr, old_size, new_size);
	if (!alloc)
	{
		perror("ft_realloc");
		exit(1);
	}
	return (alloc);
}

void	double_free(void **ptr, size_t size_if_not_null_term)
{
	size_t	i;

	if (!ptr)
		return ;
	i = 0;
	if (!size_if_not_null_term)
	{
		while (ptr[i])
			free(ptr[i++]);
	}
	else
	{
		while (i < size_if_not_null_term)
			free(ptr[i++]);
	}
	free(ptr);
}
