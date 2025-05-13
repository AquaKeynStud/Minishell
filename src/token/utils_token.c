/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:18:47 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/13 18:39:58 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	init_s(t_lexing *s, char *input, bool is_var)
{
	s->i = 0;
	s->merge = false;
	s->is_var = is_var;
	s->quoted = false;
	s->end_quote = 0;
	s->str = input;
}

void	ajust_data(t_lexing *s)
{
	if (s->i >= s->end_quote)
	{
		s->quoted = false;
		s->end_quote = 0;
	}
	if (is_whitespace(s->str[s->i]))
	{
		s->i++;
		s->merge = false;
	}
}

t_token	*get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}
