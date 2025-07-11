/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:17:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/11 21:01:49 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	free_tokens(t_ctx *ctx, t_token **list)
{
	t_token	*current;
	t_token	*next;

	current = *list;
	while (current)
	{
		next = current->next;
		s_free(ctx, current->value);
		s_free(ctx, current);
		current = next;
	}
	*list = NULL;
}

bool	add_or_merge(t_ctx *ctx, t_token **tok, t_lexing *lx, t_token *content)
{
	t_token	*last;
	char	*merged;

	if (!content)
		return (lx->merge);
	if (!lx->merge)
	{
		add_token(tok, content);
		if (content->next)
			return (lx->merge);
		return (true);
	}
	last = get_last_token(*tok);
	if (last && last->type == TOKEN_WORD)
	{
		merged = s_save(ctx, ft_strjoin_free(ctx, last->value, content->value));
		if (merged)
			last->value = merged;
		last->next = content->next;
		s_free(ctx, content);
		return (true);
	}
	return (lx->merge);
}

t_token	*create_token(t_ctx *ctx, const char *value, t_token_type type)
{
	t_token	*token;

	token = s_malloc(ctx, sizeof(t_token));
	token->value = s_save(ctx, ft_strdup(value));
	if (!token->value)
	{
		s_free(ctx, token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	tmp = *head;
	if (!*head)
	{
		new->prev = NULL;
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

t_token	*simple_tok(t_ctx *ctx, t_lexing *lx, char **res, int len)
{
	t_token	*tok;

	tok = create_token(ctx, *res, TOKEN_WORD);
	if (len)
		lx->i -= len;
	s_free(ctx, *res);
	if (tok)
		return (tok);
	return (NULL);
}
