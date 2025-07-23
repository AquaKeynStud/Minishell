/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 02:24:51 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/23 03:04:35 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

size_t	ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] && i < n)
	{
		dst[i] = src[i];
		i++;
	}
	while (i < n)
		dst[i++] = 0;
	return (i);
}

int	wildcards_match(t_ctx *ctx, const char *pat, const char *str, int wc_i)
{
	int	next_wc_i;

	if (!*pat && !*str)
		return (1);
	if (*pat == '*')
	{
		if (ctx->is_quoted[wc_i])
		{
			if (*str != '*')
				return (0);
			return (wildcards_match(ctx, pat + 1, str + 1, wc_i + 1));
		}
		next_wc_i = wc_i;
		while (*(pat + 1) == '*')
		{
			pat++;
			next_wc_i++;
		}
		if (!*(pat + 1))
			return (1);
		while (*str)
		{
			if (wildcards_match(ctx, pat + 1, str, next_wc_i + 1))
				return (1);
			str++;
		}
		return (0);
	}
	if (*pat == *str)
		return (wildcards_match(ctx, pat + 1, str + 1, wc_i));
	return (0);
}



int	contains_wildcard(const char *s)
{
	while (s && *s)
	{
		if (*s == '*')
			return (1);
		s++;
	}
	return (0);
}


char	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strcat(char *dest, const char *src)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j])
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

void expand_last_token_if_needed(t_ctx *ctx, t_token **tokens)
{
	t_token	*last;
	t_token	*expanded;

	if (!ctx->has_wildcard || !tokens || !*tokens)
		return;
	t_token **cur = tokens;
	while ((*cur)->next)
		cur = &(*cur)->next;
	last = *cur;
	expanded = expand_wildcards(ctx, last->value);
	if (!expanded)
	{
		// Soit garder le token original, soit le supprimer
		return;
	}
	free_token(ctx, last);
	*cur = expanded;
	ctx->has_wildcard = false;
}

void	free_token(t_ctx *ctx, t_token *tok)
{
	if (!tok)
		return;
	if (tok->value)
		s_free(ctx, tok->value);
	tok->next = NULL;
	s_free(ctx, tok);
}

int	count_wildcards(char *input)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '*')
			count++;
		i++;
	}
	return (count);
}

void	init_is_quote(t_ctx *ctx, char *input)
{
	int		i;
	int		wildcard_index;
	bool	quoted;
	
	i = 0;
	wildcard_index = 0;
	quoted = false;
	while (input[i])
	{
		if (input[i] == '"')
			quoted = !quoted;
		else if (input[i] == '*')
			ctx->is_quoted[wildcard_index++] = quoted;
		i++;
	}
}

