/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:25:51 by arocca            #+#    #+#             */
/*   Updated: 2025/08/15 13:51:54 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parsing.h"
#include "minishell.h"

static void	merge_splitted(t_ctx *ctx, char **table, t_ast *parent, t_ast *ast)
{
	int		i;
	t_token	*tmp;

	i = 1;
	while (table[i])
	{
		tmp = new_token(ctx, table[i++], TOKEN_WORD, NONE);
		if (!parent)
			ast_add(ctx, ast, new_ast(ctx, AST_COMMAND,
					set_merge_value(&tmp, true)), true);
		else
			ast_add(ctx, parent, new_ast(ctx, AST_COMMAND,
					set_merge_value(&tmp, true)), false);
		free_tokens(ctx, &tmp);
	}
}

void	split_ifs(t_ctx *ctx, t_ast *parent, t_ast *ast)
{
	char	*ifs;
	char	**splitted;

	if (ast->quote != NONE || !ast->value || !*ast->value)
		return ;
	ifs = get_from_env(ctx->env, "IFS");
	if (!ifs || !*ifs)
		ifs = " \t\n";
	splitted = ft_split_str(ast->value, ifs);
	s_free(ctx, ast->value);
	ast->value = s_save(ctx, ft_strdup(splitted[0]));
	merge_splitted(ctx, splitted, parent, ast);
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
	if (!file->value || !*file->value)
		remove_ast_child(ctx, node, 0);
	if (node->sub_count >= 2 && !cmd->has_space)
	{
		file->value = ft_strjoin_free(ctx, file->value, cmd->value);
		cmd->value = NULL;
		remove_ast_child(ctx, node, 1);
	}
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
