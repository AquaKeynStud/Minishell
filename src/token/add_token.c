/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:17:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/01 19:35:54 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

/* Si le dernier token est un mot, on le fusionne; sinon on en recrée un */
void	add_or_merge_word(t_token **tokens, char *content, bool merge)
{
	t_token	*tok;
	t_token	*last;
	char	*merged;

	if (!content)
		return ;
	if (merge)
	{
		last = get_last_token(*tokens);
		if (last && last->type == TOKEN_WORD)
		{
			merged = ft_strjoin(last->value, content);
			if (merged)
			{
				free(last->value);
				last->value = merged;
			}
			free(content);
			return ;
		}
	}
	tok = create_token(content, TOKEN_WORD); /* sinon on crée un nouveau token */
	if (tok)
		add_token(tokens, tok); /* create_token prend possession de content */
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
