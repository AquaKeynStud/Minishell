/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:17:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/12 11:47:00 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	free_tokens(t_token **list)
{
	t_token	*current;
	t_token	*next;

	current = *list;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*list = NULL;
}

bool	add_or_merge_word(t_token **tokens, t_lexing *lx, t_token *content)
{
	t_token	*last;
	char	*merged;

	if (!content)
		return (lx->merge);
	if (!lx->merge)
	{
		add_token(tokens, content);
		if (content->next)
			return (lx->merge);
		return (true);
	}
	last = get_last_token(*tokens);
	if (last && last->type == TOKEN_WORD)
	{
		merged = ft_strjoin_free(last->value, content->value);
		if (merged)
			last->value = merged;
		last->next = content->next;
		free(content);
		return (true);
	}
	return (lx->merge);
}

t_token	*create_token(const char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
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

t_token	*simple_tok(t_lexing *lx, char **res, int len)
{
	t_token	*tok;

	tok = create_token(*res, TOKEN_WORD);
	if (len)
		lx->i -= len;
	free(*res);
	if (tok)
		return (tok);
	return (NULL);
}
