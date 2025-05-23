/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_semantics.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:37:20 by arocca            #+#    #+#             */
/*   Updated: 2025/05/01 23:33:21 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

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

void	redir_priority(t_ast **cmd, t_ast *redir)
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
		ast_add_child(redir, leaf);
		parent->childs[1] = redir;
	}
	else
	{
		ast_add_child(redir, *cmd);
		*cmd = redir;
	}
}

t_ast	*overwrite_stub(t_token **curr, t_ast **cmd)
{
	t_ast	*stub;

	if (!*cmd)
		*cmd = new_ast(AST_COMMAND, (*curr)->value);
	else
	{
		stub = *cmd;
		while (stub->type == AST_REDIR)
			stub = stub->childs[1];
		stub->value = ft_strdup((*curr)->value);
	}
	stub = *cmd;
	while (stub->type == AST_REDIR)
		stub = stub->childs[1];
	*curr = (*curr)->next;
	return (stub);
}

void	cat_empty_heredoc(t_ast **cmd, t_token *tmp)
{
	t_ast	*stub;

	if (!*cmd)
	{
		if (tmp->type == TOKEN_HEREDOC)
			*cmd = new_ast(AST_COMMAND, "cat");
		else
			*cmd = new_ast(AST_COMMAND, NULL);
		return ;
	}
	if (tmp->type == TOKEN_HEREDOC)
	{
		stub = *cmd;
		while (stub->type == AST_REDIR)
			stub = stub->childs[1];
		if (!stub->value)
			stub->value = ft_strdup("cat");
	}
}
