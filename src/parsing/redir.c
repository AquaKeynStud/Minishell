/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:02:24 by arocca            #+#    #+#             */
/*   Updated: 2025/05/11 11:17:05 by arocca           ###   ########.fr       */
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

static int	err_redir(t_ctx *ctx, t_token *tmp, t_token **curr)
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

int	parse_redirs(t_ctx *ctx, t_ast **cmd, t_token **curr)
{
	int		err;
	t_token	*tmp;
	t_ast	*redir;
	t_ast	*file_node;

	if (!*curr)
		return (1);
	tmp = *curr;
	*curr = (*curr)->next;
	err = err_redir(ctx, tmp, curr);
	if (err != 2)
		return (err);
	cat_empty_heredoc(cmd, tmp);
	redir = new_ast(AST_REDIR, tmp->value);
	file_node = new_ast(AST_COMMAND, (*curr)->value);
	ast_add_child(redir, file_node);
	redir_priority(cmd, redir);
	*curr = (*curr)->next;
	return (1);
}
