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
	ctx->has_found_err = false;
	ctx->err_in_tokens = false;
	*list = NULL;
}

t_token	*set_merge_value(t_token **token, bool has_space)
{
	if (!*token)
		return (*token);
	(*token)->has_space = has_space;
	return (*token);
}

t_token	*new_token(t_ctx *ctx, char *value, t_token_type type, t_quote quote)
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
	token->quote = quote;
	token->has_space = false;
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
