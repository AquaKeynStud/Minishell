/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:37:36 by arocca            #+#    #+#             */
/*   Updated: 2025/07/29 09:06:19 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <stdbool.h>

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

static int skip_word(t_lexing *s)
{
	int	start;
	
	start = s->i;
	while (s->str[s->i] && !is_whitespace(s->str[s->i])
		&& (s->is_var || !is_operator(s->str[s->i]))
		&& (s->is_var || (s->str[s->i] != '(' && s->str[s->i] != ')')))
	{
		if (s->str[s->i] == '"' || s->str[s->i] == '\'')
			break ;
		(s->i)++;
	}
	return (s->i - start);
}

static void handle_word(t_ctx *ctx, t_lexing *s, t_token **tokens)
{
	int		start;
	int		len;
	char	*str;
	t_token	*expanded;

	start = s->i;
	len = skip_word(s);
	if (len > 0)
	{
		str = s_save(ctx, ft_strndup(&s->str[start], len));
		if (!str)
			return ;
		if (ft_strchr(str, '*'))
			ctx->has_wildcard = true;
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
	ctx->is_quoted = s_malloc(ctx, count_wildcards(input) * sizeof(bool));
	while (input[s.i])
	{
		ajust_data(&s);
		init_is_quote(ctx, input);
		if (s.i < s.end_quote && s.quoted)
			handle_quotes(ctx, &s, &tokens, '"');
		else if (input[s.i] == '"' || input[s.i] == '\'')
			handle_quotes(ctx, &s, &tokens, input[s.i]);
		else if (!is_var && (input[s.i] == '|' || input[s.i] == '&'))
		{
			if (ctx->has_wildcard)
				expand_last_token_if_needed(ctx, &tokens);
			handle_bonus(ctx, &s, &tokens, input[s.i]);
		}
		else if (!is_var && (input[s.i] == '>' || input[s.i] == '<'))
		{
			if (ctx->has_wildcard)
				expand_last_token_if_needed(ctx, &tokens);
			handle_redir(ctx, &s, &tokens);
		}
		else if (!is_var && (input[s.i] == '(' || input[s.i] == ')'))
		{
			if (ctx->has_wildcard)
				expand_last_token_if_needed(ctx, &tokens);
			handle_parenthesis(ctx, &s, &tokens);
		}
		else
			handle_word(ctx, &s, &tokens);
	}
	if (ctx->has_wildcard)
		expand_last_token_if_needed(ctx, &tokens);
	return (tokens);
}
