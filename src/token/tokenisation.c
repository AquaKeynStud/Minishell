/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:37:36 by arocca            #+#    #+#             */
/*   Updated: 2025/07/13 08:43:28 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <stdbool.h>

static void	handle_bonus(t_ctx *ctx, t_lexing *s, t_token **tokens, char op)
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

static void	handle_redir(t_ctx *ctx, t_lexing *s, t_token **tokens)
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

static void	handle_quotes(t_ctx *ctx, t_lexing *s, t_token **tokens, char quote)
{
	int		len;
	int		start;
	char	*content;
	t_token	*expanded;

	if (!s->quoted)
		(s->i)++;
	start = s->i;
	while (s->str[s->i] && s->str[s->i] != quote)
		(s->i)++;
	if (s->str[s->i] != quote)
		return ;
	len = s->i - start;
	content = s_save(ctx, ft_strndup(&s->str[start], len));
	(s->i)++;
	if (!content || len < 0)
		return ;
	s->quoted = true;
	s->end_quote = s->i;
	if (quote == '"' && len > 0)
		expanded = expand_args(ctx, *tokens, s, content);
	else
		expanded = create_token(ctx, content, TOKEN_WORD);
	s_free(ctx, content);
	s->merge = add_or_merge(ctx, tokens, s, expanded);
}

static void	handle_word(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	int		len;
	char	*str;
	int		start;
	t_token	*expanded;

	start = s->i;
	while (s->str[s->i] && !is_whitespace(s->str[s->i])
		&& (s->is_var || !is_operator(s->str[s->i]))
		&& (s->is_var || (s->str[s->i] != '(' && s->str[s->i] != ')')))
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
		expanded = expand_args(ctx, *tokens, s, str);
		s_free(ctx, str);
		s->merge = add_or_merge(ctx, tokens, s, expanded);
	}
}

t_token	*tokenize(t_ctx *ctx, char *input, bool is_var)
{
	t_lexing	s;
	t_token		*tokens;

	tokens = NULL;
	init_s(&s, input, is_var);
	while (input[s.i])
	{
		ajust_data(&s);
		if (s.i < s.end_quote && s.quoted)
			handle_quotes(ctx, &s, &tokens, '"');
		else if (input[s.i] == '"' || input[s.i] == '\'')
			handle_quotes(ctx, &s, &tokens, input[s.i]);
		else if (!is_var && (input[s.i] == '|' || input[s.i] == '&'))
			handle_bonus(ctx, &s, &tokens, input[s.i]);
		else if (!is_var && (input[s.i] == '>' || input[s.i] == '<'))
			handle_redir(ctx, &s, &tokens);
		else if (!is_var && (input[s.i] == '(' || input[s.i] == ')'))
			handle_parenthesis(ctx, &s, &tokens);
		else
			handle_word(ctx, &s, &tokens);
	}
	return (tokens);
}
