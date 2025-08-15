/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 21:48:12 by arocca            #+#    #+#             */
/*   Updated: 2025/08/15 12:18:56 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	handle_bonus(t_ctx *ctx, t_lexing *s, t_token **tokens, char op)
{
	int		i;
	t_token	*new;
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
			new = new_token(ctx, value, TOKEN_OR, NONE);
		else
			new = new_token(ctx, value, TOKEN_PIPE, NONE);
	}
	else
		new = new_token(ctx, value, TOKEN_AND, NONE);
	set_merge_value(&new, true);
	add_token(tokens, new);
	s_free(ctx, value);
}

void	handle_parenthesis(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	char	c;
	t_token	*token;

	c = s->str[s->i];
	if (c == '(')
		token = new_token(ctx, "(", TOKEN_LPAR, NONE);
	else
		token = new_token(ctx, ")", TOKEN_RPAR, NONE);
	if (token)
	{
		set_merge_value(&token, true);
		add_token(tokens, token);
	}
	(s->i)++;
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
		token = new_token(ctx, str, TOKEN_REDIR_APPEND, NONE);
	else if (op == '<' && len == 2)
		token = new_token(ctx, str, TOKEN_HEREDOC, NONE);
	else if (op == '>')
		token = new_token(ctx, str, TOKEN_REDIR_OUT, NONE);
	else
		token = new_token(ctx, str, TOKEN_REDIR_IN, NONE);
	if (token)
	{
		set_merge_value(&token, true);
		add_token(tokens, token);
	}
	s_free(ctx, str);
	s->i += len;
}

void	handle_quotes(t_ctx *ctx, t_lexing *s, t_token **tokens, char quote)
{
	int		len;
	t_token	*new;
	int		start;
	char	*content;

	(s->i)++;
	start = s->i;
	new = NULL;
	while (s->str[s->i] && s->str[s->i] != quote)
		(s->i)++;
	if (s->str[s->i] != quote)
		return ;
	len = s->i - start;
	content = s_save(ctx, ft_strndup(&s->str[start], len));
	(s->i)++;
	if (!content || len < 0)
		return ;
	if (quote == '"')
		new = new_token(ctx, content, TOKEN_WORD, DOUBLE);
	else
		new = new_token(ctx, content, TOKEN_WORD, SINGLE);
	s_free(ctx, content);
	if (new)
		add_token(tokens, set_merge_value(&new, s->has_space));
	s->has_space = false;
}

void	handle_word(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	int		len;
	char	*str;
	t_token	*new;
	int		start;

	start = s->i;
	while (s->str[s->i] && !is_whitespace(s->str[s->i]) && !is_op(s->str[s->i])
		&& (s->str[s->i] != '(' && s->str[s->i] != ')'))
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
		new = new_token(ctx, str, TOKEN_WORD, NONE);
		s_free(ctx, str);
		if (new)
			add_token(tokens, set_merge_value(&new, s->has_space));
		s->has_space = false;
	}
}
