/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:25:51 by arocca            #+#    #+#             */
/*   Updated: 2025/08/18 23:27:01 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parsing.h"
#include "minishell.h"

static void	merge_splitted(t_ctx *ctx, char **table, t_ast *parent, int index)
{
	int		i;
	t_token	*tmp;

	i = 0;
	if (index == -1)
		i = 1;
	while (table[i])
	{
		tmp = new_token(ctx, table[i], TOKEN_WORD, NONE);
		ast_add(ctx, parent, new_ast(ctx, AST_COMMAND,
				set_merge_value(&tmp, true)), index + i);
		free_tokens(ctx, &tmp);
		i++;
	}
}

void	split_ifs(t_ctx *ctx, t_ast *parent, t_ast *ast, int index)
{
	char	*ifs;
	char	**splitted;

	if (ast->quote != NONE || !ast->value || !*ast->value)
		return ;
	ifs = get_from_env(ctx->env, "IFS");
	if (!ifs || !*ifs)
		ifs = " \t\n";
	splitted = ft_split_str(ast->value, ifs);
	if ((!splitted[1] || !*splitted[1]) || !parent)
	{
		s_free(ctx, ast->value);
		ast->value = s_save(ctx, ft_strdup(splitted[0]));
		if (!parent)
			merge_splitted(ctx, splitted, ast, -1);
	}
	else
	{
		remove_ast_child(ctx, parent, index);
		merge_splitted(ctx, splitted, parent, index);
	}
	double_free(ctx, (void **)splitted, 0);
	return ;
}

void	merge_redir(t_ctx *ctx, t_ast *node)
{
	t_ast	*cmd;
	t_ast	*file;

	file = node->childs[0];
	cmd = node->childs[1];
	if (!node->childs || node->sub_count < 2)
		return ;
	merge_ast(ctx, cmd);
	if ((!file->value || !*file->value) && file->sub_count && file->childs[0])
	{
		file->value = s_save(ctx, ft_strdup(file->childs[0]->value));
		remove_ast_child(ctx, file, 0);
	}
	while (file->sub_count && !file->childs[0]->has_space)
	{
		file->value = s_save(ctx,
				ft_strjoin(file->value, file->childs[0]->value));
		remove_ast_child(ctx, file, 0);
	}
	if (!file->value || !*file->value)
		remove_ast_child(ctx, node, 0);
}

static void	merge_childs(t_ctx *ctx, t_ast *node)
{
	int		i;
	t_ast	*curr;
	t_ast	*next;

	i = 0;
	while (i < node->sub_count - 1)
	{
		curr = node->childs[i];
		next = node->childs[i + 1];
		if (!curr || !next)
		{
			i++;
			continue ;
		}
		if (next->value && !next->has_space)
		{
			curr->value = ft_strjoin_free(ctx, curr->value, next->value);
			curr->quote = next->quote;
			next->value = NULL;
			remove_ast_child(ctx, node, i + 1);
		}
		else
			i++;
	}
}

void	merge_ast(t_ctx *ctx, t_ast *node)
{
	if (!node || !node->childs)
		return ;
	if (node->type != AST_COMMAND && node->type != AST_REDIR)
		return ;
	if (node->type == AST_REDIR)
		return (merge_redir(ctx, node));
	if (node->value && node->sub_count == 1 && !node->childs[0]->has_space)
	{
		node->value = ft_strjoin_free(ctx, node->value, node->childs[0]->value);
		node->quote = node->childs[0]->quote;
		node->childs[0]->value = NULL;
		remove_ast_child(ctx, node, 0);
	}
	merge_childs(ctx, node);
	if (node->value && node->sub_count > 0 && !node->childs[0]->has_space)
	{
		node->value = ft_strjoin_free(ctx, node->value, node->childs[0]->value);
		node->quote = node->childs[0]->quote;
		node->childs[0]->value = NULL;
		remove_ast_child(ctx, node, 0);
	}
}
