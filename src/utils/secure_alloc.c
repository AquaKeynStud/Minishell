/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 00:12:01 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include "parsing.h"

int	set_status(t_ctx *ctx, int value)
{
	static int	*status = NULL;

	if (ctx)
		status = &ctx->status;
	else
		*status = value;
	return (value);
}

void	*s_malloc(t_ctx *ctx, size_t size)
{
	t_allocs	*new;
	void		*alloc;

	if (!size)
		return (NULL);
	alloc = malloc(size);
	if (!alloc)
	{
		perror("malloc");
		secure_exit(ctx);
	}
	new = malloc(sizeof(t_allocs));
	if (!new)
	{
		perror("garbage collector");
		s_free(ctx, alloc);
		secure_exit(ctx);
	}
	new->alloc = alloc;
	new->next = ctx->allocs;
	ctx->allocs = new;
	return (alloc);
}

void	*s_realloc(t_ctx *ctx, void *ptr, size_t old_size, size_t new_size)
{
	t_allocs	*new;
	void		*alloc;

	if (!new_size)
		return (NULL);
	s_delete(ctx, ptr);
	alloc = ft_realloc(ptr, old_size, new_size);
	if (!alloc)
	{
		perror("ft_realloc");
		secure_exit(ctx);
	}
	new = malloc(sizeof(t_allocs));
	if (!new)
	{
		perror("garbage collector");
		s_free(ctx, alloc);
		secure_exit(ctx);
	}
	new->alloc = alloc;
	new->next = ctx->allocs;
	ctx->allocs = new;
	return (alloc);
}

void	double_free(t_ctx *ctx, void **ptr, size_t size_if_not_null_term)
{
	size_t	i;

	if (!ptr)
		return ;
	i = 0;
	if (!size_if_not_null_term)
	{
		while (ptr[i])
			s_free(ctx, ptr[i++]);
	}
	else
	{
		while (i < size_if_not_null_term)
			s_free(ctx, ptr[i++]);
	}
	s_free(ctx, ptr);
}
