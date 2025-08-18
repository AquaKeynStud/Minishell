/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:55:02 by arocca            #+#    #+#             */
/*   Updated: 2025/08/18 20:47:10 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
	cat_empty_heredoc(ctx, cmd, tmp);
	redir = new_ast(ctx, AST_REDIR, tmp);
	file_node = new_ast(ctx, AST_COMMAND, *curr);
	ast_add(ctx, redir, file_node, -1);
	redir_priority(ctx, cmd, redir);
	*curr = (*curr)->next;
	return (1);
}

bool	parse_parenthesis(t_ctx *ctx, t_token **curr, t_ast **cmd)
{
	t_ast	*subcommand;

	*cmd = NULL;
	subcommand = NULL;
	if (!*curr || (*curr)->type != TOKEN_LPAR)
		return (false);
	s_free(ctx, (*curr)->value);
	(*curr)->value = s_save(ctx, ft_strdup("()"));
	*cmd = new_ast(ctx, AST_SUB, *curr);
	*curr = (*curr)->next;
	subcommand = parse_logical(ctx, curr);
	if (!subcommand || !*curr || (*curr)->type != TOKEN_RPAR)
	{
		parsing_err(ctx, (*curr)->value, 1);
		if (subcommand)
			free_ast(ctx, subcommand);
		return (false);
	}
	*curr = (*curr)->next;
	ast_add(ctx, *cmd, subcommand, -1);
	return (true);
}

static t_ast	*parse_command(t_ctx *ctx, t_token **curr, t_ast *stub)
{
	t_ast	*cmd;

	if (parse_parenthesis(ctx, curr, &cmd))
		return (cmd);
	while (*curr && !is_binary_op((*curr)->type) && (*curr)->type != TOKEN_RPAR)
	{
		if ((*curr)->type != TOKEN_WORD && (*curr)->type != TOKEN_PIPE)
		{
			if (!parse_redirs(ctx, &cmd, curr))
				return (free_ast(ctx, cmd));
		}
		else if ((*curr)->type == TOKEN_WORD)
		{
			if (!cmd || !stub)
				stub = overwrite_stub(ctx, curr, &cmd);
			else
			{
				ast_add(ctx, stub, new_ast(ctx, AST_COMMAND, *curr), -1);
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
	t_token	*tmp;
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(ctx, curr, NULL);
	if (!left)
		return (NULL);
	if (!left->value)
		return (free_ast(ctx, left));
	while (*curr && (*curr)->type == TOKEN_PIPE)
	{
		tmp = *curr;
		*curr = (*curr)->next;
		right = parse_command(ctx, curr, NULL);
		if (!right || !right->value)
			return (double_free_ast(ctx, right, left));
		pipe_node = new_ast(ctx, AST_PIPE, tmp);
		ast_add(ctx, pipe_node, left, -1);
		ast_add(ctx, pipe_node, right, -1);
		left = pipe_node;
	}
	return (left);
}

t_ast	*parse_logical(t_ctx *ctx, t_token **curr)
{
	t_token	*tmp;
	t_ast	*left;
	t_ast	*right;
	t_ast	*logical_node;

	left = parse_pipeline(ctx, curr);
	if (!left)
		return (NULL);
	while (*curr && ((*curr)->type == TOKEN_AND || (*curr)->type == TOKEN_OR))
	{
		tmp = *curr;
		*curr = (*curr)->next;
		right = parse_pipeline(ctx, curr);
		if (!right)
			return (double_free_ast(ctx, right, left));
		if (tmp->type == TOKEN_OR)
			logical_node = new_ast(ctx, AST_OR, tmp);
		else
			logical_node = new_ast(ctx, AST_AND, tmp);
		ast_add(ctx, logical_node, left, -1);
		ast_add(ctx, logical_node, right, -1);
		left = logical_node;
	}
	return (left);
}
