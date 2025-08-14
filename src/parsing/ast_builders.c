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

t_ast	*new_ast(t_ctx *ctx, t_ast_type type, t_token *curr)
{
	t_ast	*node;

	node = (t_ast *)s_malloc(ctx, sizeof(t_ast));
	node->fd = -1;
	node->type = type;
	node->value = NULL;
	node->childs = NULL;
	node->sub_count = 0;
	node->quote = curr->quote;
	node->has_space = curr->has_space;
	if (curr && curr->value)
		node->value = s_save(ctx, ft_strdup(curr->value));
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

void	ast_add_first(t_ctx *ctx, t_ast *parent, t_ast *child)
{
	int		i;
	size_t	old_size;
	size_t	new_size;

	old_size = sizeof(t_ast *) * parent->sub_count;
	new_size = sizeof(t_ast *) * (++parent->sub_count);
	parent->childs = s_realloc(ctx, parent->childs, old_size, new_size);
	i = parent->sub_count - 1;
	while (i > 0)
	{
		parent->childs[i] = parent->childs[i - 1];
		parent->childs[i - 1] = NULL;
		i--;
	}
	parent->childs[0] = child;
}

void	remove_ast_child(t_ctx *ctx, t_ast *parent, int index)
{
	int	i;

	if (!parent || index < 0 || index >= parent->sub_count)
		return ;
	free_ast(ctx, parent->childs[index]);
	i = index;
	while (i < parent->sub_count - 1)
	{
		parent->childs[i] = parent->childs[i + 1];
		parent->childs[i]->sub_count--;
		i++;
	}
	parent->sub_count--;
	if (parent->sub_count == 0)
	{
		s_free(ctx, parent->childs);
		parent->childs = NULL;
	}
	else
		parent->childs = s_realloc(ctx, parent->childs, sizeof(t_ast *) * (parent->sub_count + 1), sizeof(t_ast *) * parent->sub_count);
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
