/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 02:24:51 by abouclie          #+#    #+#             */
/*   Updated: 2025/08/01 11:34:04 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	free_token(t_ctx *ctx, t_token *tok)
{
	if (!tok)
		return ;
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
