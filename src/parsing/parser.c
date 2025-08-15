/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:49:18 by arocca            #+#    #+#             */
/*   Updated: 2025/07/12 21:02:54 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

t_ast	*overwrite_stub(t_ctx *ctx, t_token **curr, t_ast **cmd)
{
	t_ast	*stub;

	if (!*cmd)
		*cmd = new_ast(ctx, AST_COMMAND, *curr);
	stub = *cmd;
	while (stub->type == AST_REDIR)
	{
		if (!stub->childs[1])
			break ;
		stub = stub->childs[1];
	}
	if (stub->value)
		s_free(ctx, stub->value);
	stub->value = s_save(ctx, ft_strdup((*curr)->value));
	*curr = (*curr)->next;
	return (stub);
}

t_ast	*parse_input(t_ctx *ctx, t_token *tokens)
{
	t_ast	*ast;
	t_token	*curr;

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
