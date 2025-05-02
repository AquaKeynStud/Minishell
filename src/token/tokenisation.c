/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:37:36 by arocca            #+#    #+#             */
/*   Updated: 2025/05/02 00:38:32 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <stdbool.h>

static void	handle_redir(t_lexing *s, t_token **tokens)
{
	char	op;
	int		len;
	char	*str;
	t_token	*token;

	len = 1;
	op = s->input[s->i];
	if (s->input[s->i + 1] == op)
		len = 2;
	str = ft_strndup(&s->input[s->i], len);
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
	char	*expanded;

	(s->i)++;
	start = s->i;
	while (s->input[s->i] && s->input[s->i] != quote)
		(s->i)++;
	if (s->input[s->i] != quote)
		return ;
	len = s->i - start;
	content = ft_strndup(&s->input[start], len);
	(s->i)++;
	if (!content)
		return ;
	if (quote == '"')
	{
		expanded = expand_args(ctx, content);
		free(content);
	}
	else
		expanded = content;
	add_or_merge_word(tokens, expanded, s->merge);
	s->merge = true;
}

static void	handle_pipe(t_lexing *s, t_token **tokens)
{
	add_token(tokens, create_token("|", TOKEN_PIPE));
	s->i++;
	s->merge = false;
}

static void	handle_word(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	int		len;
	char	*str;
	int		start;
	char	*expanded;

	start = s->i;
	while (s->input[s->i] && !is_whitespace(s->input[s->i])
		&& !is_operator(s->input[s->i]))
	{
		if (s->input[s->i] == '"' || s->input[s->i] == '\'')
			break ;
		(s->i)++;
	}
	len = s->i - start;
	if (len > 0)
	{
		str = ft_strndup(&s->input[start], len);
		if (!str)
			return ;
		expanded = expand_args(ctx, str);
		free(str);
		if (expanded)
			add_or_merge_word(tokens, expanded, s->merge);
	}
	s->merge = true;
}

t_token	*tokenize(t_ctx *ctx, char *input)
{
	t_lexing	s;
	t_token		*tokens;

	tokens = NULL;
	init_s(&s, input);
	while (input[s.i])
	{
		if (is_whitespace(input[s.i]))
		{
			s.i++;
			s.merge = false;
		}
		if (input[s.i] == '"' || input[s.i] == '\'')
			handle_quotes(ctx, &s, &tokens, input[s.i]);
		else if (input[s.i] == '|')
			handle_pipe(&s, &tokens);
		else if (input[s.i] == '>' || input[s.i] == '<')
			handle_redir(&s, &tokens);
		else
			handle_word(ctx, &s, &tokens);
	}
	return (tokens);
}
