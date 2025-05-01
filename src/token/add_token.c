/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:17:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/01 11:17:37 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

/* Concatène new_str à last->content (qui doit être TOKEN_WORD) */
void	merge_word_token(t_token *last, const char *new_str)
{
	char	*merged;
	size_t	old_len;
	size_t	add_len;

	old_len = ft_strlen(last->value);
	add_len = ft_strlen(new_str);
	merged = malloc(old_len + add_len + 1);
	if (!merged)
		return ;
	ft_memcpy(merged, last->value, old_len);
	ft_memcpy(merged + old_len, new_str, add_len + 1);
	free(last->value);
	last->value = merged;
}

/* Si le dernier token est un mot, on le fusionne; sinon on en recrée un */
void	add_or_merge_word(t_token **tokens, char *content, bool merge)
{
	t_token	*tok;
	t_token	*last;

	if (merge)
	{
		last = get_last_token(*tokens);
		if (last && last->type == TOKEN_WORD)
		{
			merge_word_token(last, content);
			free(content);
			return ;
		}
	}
	tok = create_token(content, TOKEN_WORD); /* sinon on crée un nouveau token */
	if (tok)
		add_token(tokens, tok); /* create_token prend possession de content */
	else
		free(content);
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
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	tmp = *head;
	if (!*head)
	{
		*head = new;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
