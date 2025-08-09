/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:18:47 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/22 02:32:51 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	init_s(t_lexing *s, char *input)
{
	s->i = 0;
	s->merge = false;
	s->str = input;
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

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
