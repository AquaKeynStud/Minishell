/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 09:36:30 by abouclie          #+#    #+#             */
/*   Updated: 2025/08/01 11:24:50 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

static int	match_wildcard_level(t_ctx *ctx, const char *pat,
				const char *str, int wc_i);

int	wildcards_match(t_ctx *ctx, const char *pat, const char *str, int wc_i)
{
	if (!*pat && !*str)
		return (1);
	if (*pat == '*')
		return (match_wildcard_level(ctx, pat, str, wc_i));
	if (*pat == *str)
		return (wildcards_match(ctx, pat + 1, str + 1, wc_i));
	return (0);
}

static int	match_wildcard_level(t_ctx *ctx, const char *pat,
				const char *str, int wc_i)
{
	int	next_wc_i;

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

void	expand_last_token_if_needed(t_ctx *ctx, t_token **tokens)
{
	t_token	*last;
	t_token	*expanded;
	t_token	**cur;

	if (!ctx->has_wildcard || !tokens || !*tokens)
		return ;
	cur = tokens;
	while ((*cur)->next)
		cur = &(*cur)->next;
	last = *cur;
	expanded = expand_wildcards(ctx, last->value);
	if (!expanded)
		return ;
	free_token(ctx, last);
	*cur = expanded;
	ctx->has_wildcard = false;
}
