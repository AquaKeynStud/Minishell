/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:46:36 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/14 12:05:47 by abouclie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"

static int	wildcard_match(const char *pattern, const char *str)
{
	if (!*pattern && !*str)
		return (1);
	if (*pattern == '*')
	{
		if (wildcard_match(pattern + 1, str))
			return (1);
		if (*str && wildcard_match(pattern, str + 1))
			return (1);
		return (0);
	}
	if (*pattern == *str)
		return (wildcard_match(pattern + 1, str + 1));
	return (0);
}

static int	is_valid_match(const char *pattern, const char *filename)
{
	if (filename[0] == '.' && pattern[0] != '.')
		return (0);
	return (wildcard_match(pattern, filename));
}

t_token	*expand_wildcard(t_ctx *ctx, const char *pattern)
{
	char		**files;
	int			i;
	t_token		*tokens;
	t_token		*tmp;

	tokens = NULL;
	files = get_files();
	if (!files)
		return (create_token(ctx, pattern, TOKEN_WORD));
	i = 0;
	while (files[i])
	{
		if (is_valid_match(pattern, files[i]))
		{
			tmp = create_token(ctx, files[i], TOKEN_WORD);
			if (!tmp)
				return (NULL);
			add_token(&tokens, tmp);
		}
		i++;
	}
	if (!tokens)
		return (create_token(ctx, pattern, TOKEN_WORD));
	return (tokens);
}
