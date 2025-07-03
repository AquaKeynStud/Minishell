/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:37:36 by arocca            #+#    #+#             */
/*   Updated: 2025/07/03 12:26:57 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <stdbool.h>

static void	handle_pipe(t_lexing *s, t_token **tokens)
{
	int		i;
	char	*pipe;

	i = 0;
	pipe = ft_strdup("");
	while (s->str[s->i] == '|')
	{
		i++;
		(s->i)++;
	}
	while (i--)
		pipe = append_char(pipe, '|');
	add_token(tokens, create_token(pipe, TOKEN_PIPE));
	free(pipe);
	s->merge = false;
}

static void	handle_redir(t_lexing *s, t_token **tokens)
{
	char	op;
	int		len;
	char	*str;
	t_token	*token;

	len = 1;
	op = s->str[s->i];
	if (s->str[s->i + 1] == op)
		len = 2;
	str = ft_strndup(&s->str[s->i], len);
	if (!str)
		return ;
	if (op == '>' && len == 2)
		token = create_token(str, TOKEN_REDIR_APPEND);
	else if (op == '<' && len == 2)
		token = create_token(str, TOKEN_HEREDOC);
	else if (op == '>')
		token = create_token(str, TOKEN_REDIR_OUT);
	else
		token = create_token(str, TOKEN_REDIR_IN);
	if (token)
		add_token(tokens, token);
	free(str);
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
	content = ft_strndup(&s->str[start], len);
	(s->i)++;
	if (!content || len < 0)
		return ;
	s->quoted = true;
	s->end_quote = s->i;
	if (quote == '"' && len > 0)
		expanded = expand_args(ctx, *tokens, s, content);
	else
		expanded = create_token(content, TOKEN_WORD);
	free(content);
	s->merge = add_or_merge_word(tokens, s, expanded);
}

static void	handle_word(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	int		len;
	char	*str;
	int		start;
	t_token	*expanded;

	start = s->i;
	while (s->str[s->i] && !is_whitespace(s->str[s->i])
		&& (s->is_var || !is_operator(s->str[s->i])))
	{
		if (s->str[s->i] == '"' || s->str[s->i] == '\'')
			break ;
		(s->i)++;
	}
	len = s->i - start;
	if (len > 0)
	{
		str = ft_strndup(&s->str[start], len);
		if (!str)
			return ;
		expanded = expand_args(ctx, *tokens, s, str);
		free(str);
		s->merge = add_or_merge_word(tokens, s, expanded);
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
		else if (!is_var && input[s.i] == '|')
			handle_pipe(&s, &tokens);
		else if (!is_var && (input[s.i] == '>' || input[s.i] == '<'))
			handle_redir(&s, &tokens);
		else
			handle_word(ctx, &s, &tokens);
	}
	return (tokens);
}
