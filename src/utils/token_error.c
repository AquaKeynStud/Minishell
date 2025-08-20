/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 10:54:02 by arocca            #+#    #+#             */
/*   Updated: 2025/08/15 12:33:15 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "parsing.h"

static bool	is_redir(t_token *token)
{
	return (
		token->type == TOKEN_HEREDOC
		|| token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_APPEND
	);
}

static bool	is_ope(t_token *token)
{
	return (
		token->type == TOKEN_OR
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_PIPE
	);
}

static bool	check_parenthesis(t_ctx *ctx, t_token *tmp, int *count)
{
	if (tmp->type == TOKEN_LPAR)
	{
		if (tmp->next->type != TOKEN_WORD && tmp->next->type != TOKEN_LPAR)
			return (parsing_err(ctx, tmp->next->value, 2));
		(*count)++;
	}
	else if (tmp->type == TOKEN_RPAR)
	{
		if ((*count) <= 0)
			return (parsing_err(ctx, tmp->value, 2));
		if (tmp->next->type == TOKEN_WORD || tmp->next->type == TOKEN_LPAR)
			return (parsing_err(ctx, tmp->next->value, 2));
		(*count)--;
	}
	return (true);
}

bool	tokens_err(t_ctx *ctx, t_token *tokens)
{
	t_token	*tmp;
	int		lpar_counter;

	tmp = tokens;
	lpar_counter = 0;
	if (is_ope(tmp) || tmp->type == TOKEN_RPAR)
		return (parsing_err(ctx, tmp->value, 2));
	while (tmp->next)
	{
		if (!check_parenthesis(ctx, tmp, &lpar_counter))
			return (false);
		else if (is_redir(tmp) && tmp->next->type != TOKEN_WORD)
			return (parsing_err(ctx, tmp->next->value, 2));
		else if (is_ope(tmp)
			&& (is_ope(tmp->next) || tmp->next->type == TOKEN_RPAR))
			return (parsing_err(ctx, tmp->next->value, 2));
		tmp = tmp->next;
	}
	if (tmp->type != TOKEN_WORD && tmp->type != TOKEN_RPAR)
		return (parsing_err(ctx, "newline", 2));
	if (tmp->type == TOKEN_RPAR && (lpar_counter--) <= 0)
		return (parsing_err(ctx, ")", 2));
	if (lpar_counter > 0)
		return (parsing_err(ctx, "(", 2));
	return (true);
}

bool	bonus_err(t_ctx *ctx, t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_AND && ft_strlen(tmp->value) != 2)
		{
			if (ft_strlen(tmp->value) > 2)
				return (parsing_err(ctx, "&&", 2));
			else if (ft_strlen(tmp->value) < 2)
				return (parsing_err(ctx, "&", 2));
		}
		else if (tmp->type == TOKEN_OR && ft_strlen(tmp->value) != 2)
			return (parsing_err(ctx, "||", 2));
		tmp = tmp->next;
	}
	return (true);
}
