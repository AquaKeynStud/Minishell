/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operator.c.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 01:29:40 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/24 01:38:50 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <stdbool.h>

void	handle_bonus(t_ctx *ctx, t_lexing *s, t_token **tokens, char op)
{
	int		i;
	char	*value;

	i = 0;
	value = s_save(ctx, ft_strdup(""));
	while (s->str[s->i] == op)
	{
		i++;
		(s->i)++;
	}
	while (i--)
		value = append_char(ctx, value, op);
	if (op == '|')
	{
		if (ft_strlen(value) > 1)
			add_token(tokens, create_token(ctx, value, TOKEN_OR));
		else
			add_token(tokens, create_token(ctx, value, TOKEN_PIPE));
	}
	else
		add_token(tokens, create_token(ctx, value, TOKEN_AND));
	s_free(ctx, value);
	s->merge = false;
}

void	handle_redir(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	char	op;
	int		len;
	char	*str;
	t_token	*token;

	len = 1;
	op = s->str[s->i];
	if (s->str[s->i + 1] == op)
		len = 2;
	str = s_save(ctx, ft_strndup(&s->str[s->i], len));
	if (!str)
		return ;
	if (op == '>' && len == 2)
		token = create_token(ctx, str, TOKEN_REDIR_APPEND);
	else if (op == '<' && len == 2)
		token = create_token(ctx, str, TOKEN_HEREDOC);
	else if (op == '>')
		token = create_token(ctx, str, TOKEN_REDIR_OUT);
	else
		token = create_token(ctx, str, TOKEN_REDIR_IN);
	if (token)
		add_token(tokens, token);
	s_free(ctx, str);
	s->i += len;
	s->merge = false;
}
