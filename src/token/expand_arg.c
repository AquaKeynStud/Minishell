/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:27:23 by arocca            #+#    #+#             */
/*   Updated: 2025/07/14 11:59:43 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

static char	*expand_status(t_ctx *ctx, t_lexing *lx, char **str, char **res)
{
	char	*s;
	char	*val;

	s = *str + 2;
	val = s_save(ctx, ft_itoa(ctx->status));
	lx->i -= (ft_strlen(s) + (lx->quoted));
	*str = s;
	return (s_save(ctx, ft_strjoin_free(ctx, *res, val)));
}

static char	*handle_env_var(t_ctx *ctx, t_lexing *lx, char **str, char *res)
{
	char	*s;
	char	*val;
	char	*key;

	s = *str + 1;
	if (*s == '?')
		return (expand_status(ctx, lx, str, &res));
	else if (ft_isdigit(*s))
		s++;
	else
	{
		while (in_str(*s, "_", true))
			s++;
	}
	key = s_save(ctx, ft_strndup(*str + 1, s - (*str + 1)));
	if (!ft_strcmp(key, "UID"))
		val = s_save(ctx, ft_strdup(ctx->uid));
	else
		val = s_save(ctx, ft_strdup(get_from_env(ctx->env, key)));
	s_free(ctx, key);
	lx->i -= ft_strlen(s);
	if (lx->quoted)
		lx->i--;
	*str = s;
	return (s_save(ctx, ft_strjoin_free(ctx, res, val)));
}

static t_token	*handle_var(t_ctx *ctx, t_lexing *lx, char **s, char **res)
{
	t_token	*tok;

	if (*res && **res)
	{
		if (lx->quoted)
			lx->i--;
		return (simple_tok(ctx, lx, res, ft_strlen(*s)));
	}
	*res = handle_env_var(ctx, lx, s, *res);
	if (!*res)
		return (NULL);
	if (!lx->quoted)
	{
		tok = tokenize(ctx, *res, true);
		if (**res && is_whitespace((*res)[ft_strlen(*res) - 1]))
			lx->merge = false;
	}
	else
		return (simple_tok(ctx, lx, res, 0));
	s_free(ctx, *res);
	if (tok)
		return (tok);
	return (NULL);
}

static t_token	*handle_quote_var(t_ctx *ctx, t_lexing *lx, char **res)
{
	char	quote;

	quote = lx->str[lx->i];
	while (lx->str[lx->i + 1] && lx->str[lx->i + 1] != quote)
	{
		lx->i++;
		*res = append_char(ctx, *res, lx->str[lx->i]);
	}
	if (!lx->str[lx->i + 1] && lx->str[lx->i] != quote)
	{
		lx->i++;
		parsing_err(ctx, "newline", 2);
		ctx->err_in_tokens = true;
		s_free(ctx, *res);
		return (NULL);
	}
	lx->i += 2;
	return (simple_tok(ctx, lx, res, 0));
}

t_token	*expand_args(t_ctx *ctx, t_token *tok, t_lexing *lx, char *s)
{
	char	*res;

	if (!s || !*s)
		return (NULL);
	res = s_save(ctx, ft_strdup(""));
	if (!res)
		return (NULL);
	if (ft_strchr(s, '*') && !lx->quoted)
		return (expand_wildcard(ctx, s));
	while (*s)
	{
		if (*s == '$' && !is_eof(tok) && in_str(s[1], "?_", true))
			return (handle_var(ctx, lx, &s, &res));
		else if (*s == '$' && !is_eof(tok) && in_str(lx->str[lx->i], "\"'", 0))
			return (handle_quote_var(ctx, lx, &res));
		else if (*s == '~' && !is_eof(tok) && !lx->quoted
			&& !*res && (!s[1] || s[1] == '/'))
			return (expand_tilde(ctx, lx, &s, &res));
		res = append_char(ctx, res, *s++);
	}
	return (simple_tok(ctx, lx, &res, ft_strlen(s)));
}
