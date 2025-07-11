/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs_collector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:24:21 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 19:47:08 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_saved(t_allocs *head, void *ptr)
{
	while (head)
	{
		if (head->alloc == ptr)
			return (true);
		head = head->next;
	}
	return (false);
}

void	*s_save(t_ctx *ctx, void *ptr)
{
	t_allocs	*new;

	if (!ptr)
		return (NULL);
	else if (is_saved(ctx->allocs, ptr))
		return (ptr);
	new = malloc(sizeof(t_allocs));
	if (!new)
	{
		perror("garbage register");
		free(ptr);
		secure_exit(ctx);
	}
	new->alloc = ptr;
	new->next = ctx->allocs;
	ctx->allocs = new;
	return (ptr);
}

void	s_delete(t_ctx *ctx, void *ptr)
{
	t_allocs	*prev;
	t_allocs	*curr;

	prev = NULL;
	curr = ctx->allocs;
	while (curr)
	{
		if (curr->alloc == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				ctx->allocs = curr->next;
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	s_free(t_ctx *ctx, void *ptr)
{
	t_allocs	*prev;
	t_allocs	*curr;

	prev = NULL;
	curr = ctx->allocs;
	if (!ptr)
		return ;
	while (curr)
	{
		if (curr->alloc == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				ctx->allocs = curr->next;
			free(curr->alloc);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	free(ptr);
}

void	free_garbage(t_allocs **head)
{
	t_allocs	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		if (tmp->alloc)
			free(tmp->alloc);
		free(tmp);
		tmp = NULL;
	}
}
