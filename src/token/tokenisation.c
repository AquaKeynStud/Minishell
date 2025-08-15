/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:37:36 by arocca            #+#    #+#             */
/*   Updated: 2025/08/01 11:29:13 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <stdbool.h>

void	init_s(t_lexing *s, char *input)
{
	s->i = 0;
	s->str = input;
	s->has_space = true;
}

t_token	*get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

char	*has_expand(t_ctx *ctx, char type, char *s)
{
	int		i;
	int		len;
	char	*res;

	i = 0;
	len = 1;
	res = s_save(ctx, ft_strdup(""));
	if (!type || !s || !*s)
		return (NULL);
	while (s[i])
	{
		if ((s[i] == '~' || s[i] == '$' || s[i] == '*'))
		{
			res = s_realloc(ctx, res, len, len + 1);
			res[len - 1] = type;
			res[len] = '\0';
			len += 1;
		}
		i++;
	}
	return (res);
}

int	is_op(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*tokenize(t_ctx *ctx, char *input)
{
	t_lexing	s;
	t_token		*tokens;

	tokens = NULL;
	init_s(&s, input);
	while (input[s.i])
	{
		if (is_whitespace(input[s.i]))
		{
			s.i++;
			s.has_space = true;
		}
		else if (s.str[s.i] == '"' || s.str[s.i] == '\'')
			handle_quotes(ctx, &s, &tokens, s.str[s.i]);
		else if (s.str[s.i] == '|' || s.str[s.i] == '&')
			handle_bonus(ctx, &s, &tokens, s.str[s.i]);
		else if ((s.str[s.i] == '>' || s.str[s.i] == '<'))
			handle_redir(ctx, &s, &tokens);
		else if (s.str[s.i] == '(' || s.str[s.i] == ')')
			handle_parenthesis(ctx, &s, &tokens);
		else
			handle_word(ctx, &s, &tokens);
	}
	return (tokens);
}
