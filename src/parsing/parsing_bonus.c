/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:09:34 by arocca            #+#    #+#             */
/*   Updated: 2025/07/12 20:31:03 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	is_binary_op(t_token_type type)
{
	return (type == TOKEN_OR
		|| type == TOKEN_AND
		|| type == TOKEN_PIPE);
}

static bool	op_allow(t_token_type type)
{
	return (type == TOKEN_WORD
		|| type == TOKEN_HEREDOC
		|| type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND);
}

static int	token_validation(t_ctx *ctx, t_token **curr)
{
	t_token	*tmp;

	tmp = *curr;
	if (!tmp->next)
		return (0);
	if (is_binary_op(tmp->type) || tmp->type == TOKEN_LPAR)
	{
		if (!op_allow(tmp->next->type) && tmp->next->type != TOKEN_LPAR)
			return (parsing_err(ctx, tmp->next->value, 2));
	}
	else if (tmp->type == TOKEN_RPAR)
	{
		if (!is_binary_op(tmp->next->type) && tmp->next->type != TOKEN_RPAR)
			return (parsing_err(ctx, tmp->next->value, 2));
	}
	else if (op_allow(tmp->type) && tmp->type != TOKEN_WORD)
	{
		if (tmp->next->type != TOKEN_WORD)
			return (parsing_err(ctx, tmp->next->value, 2));
	}
	return (1);
}

bool	parse_parenthesis(t_ctx *ctx, t_token **curr, t_ast **cmd)
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
		parsing_err(ctx, (*curr)->value, 1);
		if (subcommand)
			free_ast(ctx, subcommand);
		return (false);
	}
	*curr = (*curr)->next;
	*cmd = new_ast(ctx, AST_SUB, "()");
	ast_add(ctx, *cmd, subcommand);
	return (true);
}

int	syntax_error(t_ctx *ctx, t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	if (!curr)
		return (1);
	if (is_binary_op(curr->type) || curr->type == TOKEN_RPAR)
		return (parsing_err(ctx, curr->value, 2));
	while (curr->next)
	{
		if (!token_validation(ctx, &curr))
			return (0);
		curr = curr->next;
	}
	if (curr->type != TOKEN_WORD && curr->type != TOKEN_RPAR)
		return (parsing_err(ctx, "newline", 2));
	return (1);
}
