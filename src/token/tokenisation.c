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

t_token	*tokenize(t_ctx *ctx, char *input)
{
	t_lexing	s;
	t_token		*tokens;

	tokens = NULL;
	init_s(&s, input);
	while (input[s.i])
	{
		if (is_whitespace(input[s.i]))
			s.i++;
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
