/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joins.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:31:20 by arocca            #+#    #+#             */
/*   Updated: 2025/05/20 11:19:36 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*final;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (s2);
	else if (!s2)
		return (s1);
	else if (!*s2)
	{
		free(s2);
		return (s1);
	}
	else if (!*s1)
	{
		free(s1);
		return (s2);
	}
	final = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	if (final)
		return (final);
	return (NULL);
}

char	*join_with_delim(const char *s1, const char *s2, const char *delimiter)
{
	char	*tmp;
	char	*final;

	tmp = ft_strjoin(s1, delimiter);
	if (!tmp)
		return (NULL);
	final = ft_strjoin((const char *)tmp, s2);
	free(tmp);
	if (!final)
		perror("malloc");
	return (final);
}

char	*append_char(char *res, char c)
{
	char	*tmp;
	char	*final;

	tmp = malloc(2);
	if (!tmp)
		return (NULL);
	tmp[0] = c;
	tmp[1] = '\0';
	final = ft_strjoin(res, tmp);
	free(tmp);
	if (!final)
		return (res);
	free(res);
	return (final);
}

t_token	*expand_tilde(t_ctx *ctx, t_lexing *lx, char **s, char **res)
{
	char	*home;
	char	*path;

	*s += 1;
	free(*res);
	home = ft_strdup(check_env(ctx->env, "HOME"));
	if (!home)
	{
		home = ft_strdup(*s);
		if (!home)
			return (NULL);
		return (simple_tok(lx, &home, 0));
	}
	path = ft_strjoin(home, *s);
	if (!path)
		return (simple_tok(lx, &home, 0));
	free(home);
	return (simple_tok(lx, &path, 0));
}
