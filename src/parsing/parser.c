/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:49:18 by arocca            #+#    #+#             */
/*   Updated: 2025/07/12 10:58:20 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

static t_ast	*overwrite_stub(t_ctx *ctx, t_token **curr, t_ast **cmd)
{
	t_ast	*stub;

	if (!*cmd)
		*cmd = new_ast(ctx, AST_COMMAND, (*curr)->value);
	else
	{
		stub = *cmd;
		while (stub->type == AST_REDIR)
			stub = stub->childs[1];
		if (stub->value)
			s_free(ctx, stub->value);
		stub->value = s_save(ctx, ft_strdup((*curr)->value));
	}
	stub = *cmd;
	while (stub->type == AST_REDIR)
		stub = stub->childs[1];
	*curr = (*curr)->next;
	return (stub);
}

int	has_bonus_err(t_ctx *ctx, t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE)
		{
			if (!curr->next || (curr->next && curr->next->type == TOKEN_PIPE))
				return (parsing_err(ctx, "|", 2));
			else if (ft_strcmp(curr->value, "|"))
				return (parsing_err(ctx, "||", 2));
		}
		else if (curr->type != TOKEN_WORD)
		{
			if (!curr->next)
				return (parsing_err(ctx, "newline", 2));
			else if (curr->next && curr->next->type != TOKEN_WORD)
				return (parsing_err(ctx, curr->next->value, 2));
		}
		curr = curr->next;
	}
	curr = get_last_token(tokens);
	if (curr && curr->type != TOKEN_PIPE && curr->type != TOKEN_WORD)
		return (parsing_err(ctx, "newline", 2));
	return (1);
}

static bool	parse_parenthesis(t_ctx *ctx, t_token **curr, t_ast **cmd)
{
	t_ast	*subcommand;

	*cmd = NULL;
	subcommand = NULL;
	if (!*curr || (*curr)->type != TOKEN_LPAR)
		return (false);
	*curr = (*curr)->next;
	subcommand = parse_logical(ctx, curr);
	if (!subcommand || !*curr || (*curr)->type != TOKEN_RPAR)
	{
		parsing_err(ctx, ")", 1);
		if (subcommand)
			free_ast(ctx, subcommand);
		return (false);
	}
	*curr = (*curr)->next;
	*cmd = new_ast(ctx, AST_SUB, "()");
	ast_add(ctx, *cmd, subcommand);
	return (true);
}

static t_ast	*parse_command(t_ctx *ctx, t_token **curr, t_ast *stub)
{
	t_ast	*cmd;

	if (parse_parenthesis(ctx, curr, &cmd))
		return (cmd);
	while (*curr && (*curr)->type != TOKEN_PIPE)
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
				ast_add(ctx, stub, new_ast(ctx, AST_COMMAND, (*curr)->value));
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
		pipe_node = new_ast(ctx, AST_PIPE, tmp->value);
		ast_add(ctx, pipe_node, left);
		ast_add(ctx, pipe_node, right);
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
			logical_node = new_ast(ctx, AST_OR, tmp->value);
		else
			logical_node = new_ast(ctx, AST_AND, tmp->value);
		ast_add(ctx, logical_node, left);
		ast_add(ctx, logical_node, right);
		left = logical_node;
	}
	return (left);
}

t_ast	*parse_input(t_ctx *ctx, t_token *tokens)
{
	t_ast	*ast;
	t_token	*curr;

	printf("on est à : %i\n", ctx->err_in_tokens);
	curr = tokens;
	ctx->has_found_err = false;
	if (ctx->err_in_tokens)
	{
		ctx->err_in_tokens = false;
		return (NULL);
	}
	ast = parse_logical(ctx, &curr);
	if (curr != NULL)
	{
		parsing_err(ctx, curr->value, 2);
		return (NULL);
	}
	return (ast);
}
