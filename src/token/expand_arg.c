/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:27:23 by arocca            #+#    #+#             */
/*   Updated: 2025/05/01 20:22:32 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "minishell.h"

static char	*ft_strjoin_free(char *s1, char *s2)
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

static char	*handle_env_var(t_ctx *ctx, size_t *i, char *s, char *res)
{
	char	*val;
	char	*key;
	size_t	start;
	
	start = ++(*i);
	if (s[start] == '?')
	{
		(void)key;
		(*i)++;
		val = ft_itoa(ctx->status);
		return (ft_strjoin_free(res, val));
	}
	else
	{
		while (ft_isalnum((unsigned char)s[*i]) || s[*i] == '_')
			(*i)++;
		key = ft_strndup(&s[start], *i - start);
		if (get_from_env(ctx->env, key))
			val = ft_strdup(get_from_env(ctx->env, key));
		else
			val = ft_strdup("");
		free(key);
		return (ft_strjoin_free(res, val));
	}
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

char	*expand_args(t_ctx *ctx, char *s)
{
	size_t	i;
	char	*res;

	i = 0;
	if (!s)
		return (NULL);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1])
		{
			res = handle_env_var(ctx, &i, s, res);
			if (!res)
				return (NULL);
			continue;
		}
		res = append_char(res, s[i]);
		if (!res)
			return (NULL);
		i++;
	}
	if (res[0] == '\0')
	{
		free(res);
		return (NULL);
	}
	return (res);
}
