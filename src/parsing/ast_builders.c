/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:07:51 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 00:24:58 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

t_ast	*new_ast(t_ctx *ctx, t_ast_type type, const char *value)
{
	t_ast	*node;

	node = (t_ast *)s_malloc(ctx, sizeof(t_ast));
	node->type = type;
	node->value = NULL;
	if (value)
		node->value = s_save(ctx, ft_strdup(value));
	node->fd = -1;
	node->sub_count = 0;
	node->childs = NULL;
	return (node);
}

void	ast_add(t_ctx *ctx, t_ast *parent, t_ast *child)
{
	size_t	old_size;
	size_t	new_size;

	old_size = sizeof(t_ast *) * parent->sub_count;
	new_size = sizeof(t_ast *) * (++parent->sub_count);
	parent->childs = s_realloc(ctx, parent->childs, old_size, new_size);
	parent->childs[parent->sub_count - 1] = child;
}

void	*free_ast(t_ctx *ctx, t_ast *node)
{
	int	i;

	if (!node)
		return (NULL);
	i = 0;
	while (i < node->sub_count)
	{
		free_ast(ctx, node->childs[i]);
		i++;
	}
	s_free(ctx, node->childs);
	s_free(ctx, node->value);
	s_free(ctx, node);
	return (NULL);
}

void	*double_free_ast(t_ctx *ctx, t_ast *left, t_ast *right)
{
	free_ast(ctx, left);
	free_ast(ctx, right);
	return (NULL);
}
