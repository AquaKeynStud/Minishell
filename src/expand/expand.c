/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 06:13:33 by arocca            #+#    #+#             */
/*   Updated: 2025/08/18 23:43:16 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parsing.h"
#include "minishell.h"

bool	check_parent(t_ctx *ctx, t_ast *ast)
{
	if ((!ast->value || !*ast->value) && ast->quote == NONE)
	{
		if (!ast->childs || ast->sub_count == 0)
			return (false);
		s_free(ctx, ast->value);
		ast->value = s_save(ctx, ft_strdup(ast->childs[0]->value));
		ast->quote = ast->childs[0]->quote;
		remove_ast_child(ctx, ast, 0);
		expand_childs(ctx, ast);
		return (true);
	}
	return (true);
}

static bool	no_value_no_space(t_ctx *ctx, t_ast *ast, int *i, int checker)
{
	int		sub;
	char	*new;
	t_ast	**cur;

	cur = ast->childs;
	sub = ast->sub_count;
	if ((!cur[*i]->value || !*cur[*i]->value) && !cur[*i]->has_space)
	{
		if ((*i + 1) < sub && cur[*i]->has_space
			&& !cur[*i + 1]->has_space)
			cur[*i + 1]->has_space = cur[*i]->has_space;
		remove_ast_child(ctx, ast, *i);
		return (true);
	}
	else if (*cur[*i]->value && cur[*i]->value[checker] == '$'
		&& cur[*i]->quote == NONE && (*i + 1) < sub && !cur[*i + 1]->has_space)
	{
		new = s_save(ctx, ft_strndup(cur[*i]->value, checker));
		s_free(ctx, cur[*i]->value);
		cur[*i]->value = s_save(ctx, ft_strjoin(new, cur[*i + 1]->value));
		s_free(ctx, new);
		remove_ast_child(ctx, ast, (*i) + 1);
		return (true);
	}
	return (false);
}

static bool	expand_child(t_ctx *ctx, t_ast *node, int *i)
{
	t_ast	**childs;

	childs = node->childs;
	if (!childs[*i] || childs[*i]->type != AST_COMMAND)
		return (false);
	expand_args(ctx, childs[*i]);
	if (no_value_no_space(ctx, node, i, ft_strlen(childs[*i]->value) - 1))
		return (true);
	split_ifs(ctx, node, childs[*i], *i);
	(*i)++;
	return (false);
}

t_ast	*expand_childs(t_ctx *ctx, t_ast *node)
{
	int		i;
	t_ast	**childs;

	i = 0;
	expand_args(ctx, node);
	if (!check_parent(ctx, node))
		return (NULL);
	split_ifs(ctx, NULL, node, -1);
	while (node->childs && i < node->sub_count)
	{
		childs = node->childs;
		if (!childs[i] || childs[i]->type != AST_COMMAND)
			break ;
		if (expand_child(ctx, node, &i))
			continue ;
	}
	merge_ast(ctx, node);
	glob_ast(ctx, node);
	return (node);
}
