/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:28:18 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 21:35:31 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	handle_parenthesis(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	char	c;
	t_token	*token;
	
	c = s->str[s->i];
	if (c == '(')
		token = create_token(ctx, "(",  TOKEN_LPAR);
	else
		token = create_token(ctx, ")", TOKEN_RPAR);
	if (token)
		add_token(tokens, token);
	(s->i)++;
	s->merge = false;
}
