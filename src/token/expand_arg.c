/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:27:23 by arocca            #+#    #+#             */
/*   Updated: 2025/05/02 18:47:23 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*final;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	final = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (final);
}

static char	*handle_env_var(t_ctx *ctx, char **str, char *res)
{
	char	*s;
	char	*val;
	char	*key;

	s = *str + 1;
	if (*s == '?')
	{
		s++;
		val = ft_itoa(ctx->status);
		*str = s;
		return (ft_strjoin_free(res, val));
	}
	while (ft_isalnum(*s) || *s == '_')
		s++;
	key = ft_strndup(*str + 1, s - (*str + 1));
	if (get_from_env(ctx->env, key))
		val = ft_strdup(get_from_env(ctx->env, key));
	else
		val = ft_strdup("");
	free(key);
	*str = s;
	return (ft_strjoin_free(res, val));
}

static char	*append_char(char *res, char c)
{
	char	*tmp;

	tmp = malloc(2);
	if (!tmp)
		return (NULL);
	tmp[0] = c;
	tmp[1] = '\0';
	return (ft_strjoin_free(res, tmp));
}

static char	*handle_input_err(char *res)
{
	if (res[0] == '\0')
	{
		free(res);
		return (NULL);
	}
	return (res);
}

char	*expand_args(t_ctx *ctx, char *str)
{
	char	*s;
	char	*res;

	if (!str)
		return (NULL);
	s = str;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (*s)
	{
		if (*s == '$' && (s[1] == '?' || ft_isalnum(s[1]) || s[1] == '_'))
		{
			res = handle_env_var(ctx, &s, res);
			if (!res)
				return (NULL);
			continue ;
		}
		res = append_char(res, *s++);
		if (!res)
			return (NULL);
	}
	return (handle_input_err(res));
}
