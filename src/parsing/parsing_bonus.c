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
	ast_add(ctx, *cmd, subcommand, false);
	return (true);
}
