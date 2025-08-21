/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:02:24 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 13:05:45 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

bool	is_binary_op(t_token_type type)
{
	return (type == TOKEN_OR
		|| type == TOKEN_AND
		|| type == TOKEN_PIPE);
}

int	parsing_err(t_ctx *ctx, const char *msg, int code)
{
	char	*err;

	err = "minishell: syntax error near unexpected token `";
	if (code >= 0)
		ctx->status = code;
	if (!ctx->has_found_err)
	{
		ft_dprintf(2, "%s%s'\n", err, msg);
		ctx->has_found_err = true;
	}
	return (0);
}

int	err_redir(t_ctx *ctx, t_token *tmp, t_token **curr)
{
	if (tmp->type != TOKEN_HEREDOC && !*curr)
		return (parsing_err(ctx, "newline", 2));
	else if (tmp->type == TOKEN_HEREDOC && !*curr)
		return (1);
	else if (*curr && ((*curr)->type != TOKEN_WORD))
	{
		if ((*curr)->type != TOKEN_PIPE && !(*curr)->next)
			return (parsing_err(ctx, "newline", 2));
		else if ((*curr)->type != TOKEN_PIPE)
			return (parsing_err(ctx, (*curr)->value, 2));
	}
	return (2);
}

void	cat_empty_heredoc(t_ctx *ctx, t_ast **cmd, t_token *op)
{
	t_token	*temp;

	if (!*cmd)
	{
		temp = s_save(ctx, new_token(ctx, "", TOKEN_WORD, NONE));
		set_merge_value(&temp, true);
		*cmd = new_ast(ctx, AST_COMMAND, temp);
		s_free(ctx, temp);
	}
	(void)op;
}

void	redir_priority(t_ctx *ctx, t_ast **cmd, t_ast *redir)
{
	t_ast	*leaf;
	t_ast	*parent;

	if (*cmd && (*cmd)->type == AST_REDIR)
	{
		parent = *cmd;
		leaf = parent->childs[1];
		while (leaf && leaf->type == AST_REDIR)
		{
			parent = leaf;
			leaf = leaf->childs[1];
		}
		if (!leaf)
			return ;
		ast_add(ctx, redir, leaf, -1);
		parent->childs[1] = redir;
	}
	else
	{
		ast_add(ctx, redir, *cmd, -1);
		*cmd = redir;
	}
}
