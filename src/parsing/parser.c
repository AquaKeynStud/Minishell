/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:49:18 by arocca            #+#    #+#             */
/*   Updated: 2025/05/02 19:39:21 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

static int	parse_redirs(t_ctx *ctx, t_ast **cmd, t_token **curr)
{
	t_token	*tmp;
	t_ast	*file_node;
	t_ast	*redir;

	if (!*curr)
		return (1);
	tmp = *curr;
	*curr = (*curr)->next;
	if (*curr && ((*curr)->type != TOKEN_WORD))
	{
		if ((*curr)->type != TOKEN_PIPE)
			return (parsing_err(ctx, (*curr)->value, 2));
		return (0);
	}
	if (!*curr || ((*curr)->type != TOKEN_WORD))
		return (parsing_err(ctx, "newline", 2));
	cat_empty_heredoc(cmd, tmp);
	file_node = new_ast(AST_COMMAND, (*curr)->value);
	redir = new_ast(AST_REDIR, tmp->value);
	ast_add_child(redir, file_node);
	redir_priority(cmd, redir);
	*curr = (*curr)->next;
	return (1);
}

static t_ast	*parse_command(t_ctx *ctx, t_token **curr)
{
	t_ast	*cmd;
	t_ast	*stub;

	cmd = NULL;
	while (*curr && (*curr)->type != TOKEN_PIPE)
	{
		if ((*curr)->type != TOKEN_WORD && (*curr)->type != TOKEN_PIPE)
		{
			if (!parse_redirs(ctx, &cmd, curr))
				return (NULL);
		}
		else if ((*curr)->type == TOKEN_WORD)
		{
			if (!cmd)
				stub = overwrite_stub(curr, &cmd);
			else
			{
				ast_add_child(stub, new_ast(AST_COMMAND, (*curr)->value));
				*curr = (*curr)->next;
			}
		}
		else
			break ;
	}
	return (cmd);
}

static t_ast	*parse_pipeline(t_ctx *ctx, t_token **curr)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(ctx, curr);
	if (!left)
		return (NULL);
	if (!left->value)
		return (free_ast(left));
	while (*curr && (*curr)->type == TOKEN_PIPE)
	{
		*curr = (*curr)->next;
		right = parse_command(ctx, curr);
		if (!right || !right->value)
			return (double_free_ast(right, left));
		pipe_node = new_ast(AST_PIPE, "|");
		ast_add_child(pipe_node, left);
		ast_add_child(pipe_node, right);
		left = pipe_node;
	}
	return (left);
}

t_ast	*parse_input(t_ctx *ctx, t_token *tokens)
{
	t_ast	*ast;
	t_token	*curr;

	curr = tokens;
	ast = parse_pipeline(ctx, &curr);
	if (curr != NULL)
	{
		parsing_err(ctx, "|", 2);
		return (NULL);
	}
	return (ast);
}
