/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:25:51 by arocca            #+#    #+#             */
/*   Updated: 2025/08/14 09:22:53 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

void	split_ifs(t_ctx *ctx, t_ast *ast)
{
	int		i;
	t_token	*tmp;
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
	i = 1;
	while (splitted[i])
	{
		tmp = create_token(ctx, splitted[i], TOKEN_WORD, NONE);
		ast_add_first(ctx, ast, new_ast(ctx, AST_COMMAND, set_merge_value(&tmp, true)));
		free_tokens(ctx, &tmp);
		i++;
	}
	double_free(ctx, (void **)splitted, 0);
	return ;
}
void	merge_ast(t_ctx *ctx, t_ast *node);

void	merge_redir(t_ctx *ctx, t_ast *node)
{
	if (!node->childs || node->sub_count < 2)
		return ;
	merge_ast(ctx, node->childs[1]);
	if (!node->childs[0]->value || !*node->childs[0]->value)
		remove_ast_child(ctx, node, 0);
	if (node->sub_count >= 2 && !node->childs[1]->has_space)
	{
		node->childs[0]->value = ft_strjoin_free(ctx, node->childs[0]->value, node->childs[1]->value);
		node->childs[1]->value = NULL;
		remove_ast_child(ctx, node, 1);
	}
}

void	merge_ast(t_ctx *ctx, t_ast *node)
{
	int	i;
	t_ast *curr;
	t_ast *next;

	i = 0;
	if (!node || !node->childs || (node->type != AST_COMMAND && node->type != AST_REDIR))
		return;
	if (node->type == AST_REDIR)
		return (merge_redir(ctx, node));
	if (node->childs[0] && !node->childs[0]->has_space)
	{
		node->value = ft_strjoin_free(ctx, node->value, node->childs[0]->value);
		node->childs[0]->value = NULL;
		remove_ast_child(ctx, node, 0);
	}
	if (node->value && node->sub_count == 1 && !node->childs[0]->has_space)
	{
		node->value = ft_strjoin_free(ctx, node->value, node->childs[0]->value);
		node->childs[0]->value = NULL;
		remove_ast_child(ctx, node, 0);
	}
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
			next->value = NULL;
			remove_ast_child(ctx, node, i + 1);
		}
		else
			i++;
	}
}
