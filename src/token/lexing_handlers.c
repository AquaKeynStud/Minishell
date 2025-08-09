/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 21:48:12 by arocca            #+#    #+#             */
/*   Updated: 2025/08/09 17:15:58 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

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
			add_token(tokens, create_token(ctx, value, TOKEN_OR, NULL));
		else
			add_token(tokens, create_token(ctx, value, TOKEN_PIPE, NULL));
	}
	else
		add_token(tokens, create_token(ctx, value, TOKEN_AND, NULL));
	s_free(ctx, value);
	s->merge = false;
}

void	handle_parenthesis(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	char	c;
	t_token	*token;

	c = s->str[s->i];
	if (c == '(')
		token = create_token(ctx, "(", TOKEN_LPAR, NULL);
	else
		token = create_token(ctx, ")", TOKEN_RPAR, NULL);
	if (token)
		add_token(tokens, token);
	(s->i)++;
	s->merge = false;
}

void	handle_redir(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	char	op;
	int		len;
	char	*str;
	t_token	*token;

	op = s->str[s->i];
	len = 1 + (s->str[s->i + 1] == op);
	str = s_save(ctx, ft_strndup(&s->str[s->i], len));
	if (!str)
		return ;
	if (op == '>' && len == 2)
		token = create_token(ctx, str, TOKEN_REDIR_APPEND, NULL);
	else if (op == '<' && len == 2)
		token = create_token(ctx, str, TOKEN_HEREDOC, NULL);
	else if (op == '>')
		token = create_token(ctx, str, TOKEN_REDIR_OUT, NULL);
	else
		token = create_token(ctx, str, TOKEN_REDIR_IN, NULL);
	if (token)
		add_token(tokens, token);
	s_free(ctx, str);
	s->i += len;
	s->merge = false;
}

void	handle_quotes(t_ctx *ctx, t_lexing *s, t_token **tokens, char quote)
{
	int		len;
	int		start;
	char	*content;
	t_token	*new_token;

	(s->i)++;
	start = s->i;
	while (s->str[s->i] && s->str[s->i] != quote)
		(s->i)++;
	if (s->str[s->i] != quote)
		return ; // Ici on peut lancer en tant que mot ?
	len = s->i - start;
	content = s_save(ctx, ft_strndup(&s->str[start], len));
	(s->i)++;
	if (!content || len < 0)
		return ;
	if (len > 0 && quote == '"')
		new_token = create_token(ctx, content, TOKEN_WORD, "2");
	else if (len > 0)
		new_token = create_token(ctx, content, TOKEN_WORD, "1");
	s_free(ctx, content);
	s->merge = add_or_merge(ctx, tokens, s, new_token);
}

void	handle_word(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	int		len;
	char	*str;
	int		start;
	t_token	*new_token;

	start = s->i;
	while (s->str[s->i] && !is_whitespace(s->str[s->i]) && !is_operator(s->str[s->i]) && (s->str[s->i] != '(' && s->str[s->i] != ')'))
	{
		if (s->str[s->i] == '"' || s->str[s->i] == '\'')
			break ;
		(s->i)++;
	}
	len = s->i - start;
	if (len > 0)
	{
		str = s_save(ctx, ft_strndup(&s->str[start], len));
		if (!str)
			return ;
		new_token = create_token(ctx, str, TOKEN_WORD, "0");
		s_free(ctx, str);
		s->merge = add_or_merge(ctx, tokens, s, new_token);
	}
}
