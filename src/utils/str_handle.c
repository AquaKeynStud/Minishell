/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:31:20 by arocca            #+#    #+#             */
/*   Updated: 2025/07/12 20:19:04 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "minishell.h"

char	*ft_strjoin_free(t_ctx *ctx, char *s1, char *s2)
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
		s_free(ctx, s2);
		return (s1);
	}
	else if (!*s1)
	{
		s_free(ctx, s1);
		return (s2);
	}
	final = s_save(ctx, ft_strjoin(s1, s2));
	s_free(ctx, s1);
	s_free(ctx, s2);
	if (final)
		return (final);
	return (NULL);
}

char	*join_with_delim(t_ctx *ctx, char *s1, char *s2, char *delimiter)
{
	char	*tmp;
	char	*final;

	tmp = s_save(ctx, ft_strjoin(s1, delimiter));
	if (!tmp)
		return (NULL);
	final = s_save(ctx, ft_strjoin((char *)tmp, s2));
	s_free(ctx, tmp);
	if (!final)
		perror("malloc");
	return (final);
}

char	*append_char(t_ctx *ctx, char *res, char c)
{
	char	*tmp;
	char	*final;

	tmp = s_malloc(ctx, 2);
	if (!tmp)
		return (NULL);
	tmp[0] = c;
	tmp[1] = '\0';
	final = s_save(ctx, ft_strjoin(res, tmp));
	s_free(ctx, tmp);
	if (!final)
		return (res);
	s_free(ctx, res);
	return (final);
}

t_token	*expand_tilde(t_ctx *ctx, t_lexing *lx, char **s, char **res)
{
	char	*home;
	char	*path;

	*s += 1;
	s_free(ctx, *res);
	home = s_save(ctx, ft_strdup(check_env(ctx->env, "HOME")));
	if (!home)
	{
		home = s_save(ctx, ft_strdup(*s));
		if (!home)
			return (NULL);
		return (simple_tok(ctx, lx, &home, 0));
	}
	path = s_save(ctx, ft_strjoin(home, *s));
	if (!path)
		return (simple_tok(ctx, lx, &home, 0));
	s_free(ctx, home);
	return (simple_tok(ctx, lx, &path, 0));
}

void	print_status(t_ctx *ctx)
{
	char	*cwd;

	if (ctx && ctx->status == 0)
		ft_printf("\033[0m\033[1m\033[32m%s  ", "➜");
	else
		ft_printf("\033[1m\033[31m%s  ", "➜");
	if (ctx && get_from_env(ctx->env, "USER"))
		ft_printf("\033[35m[%s] ", get_from_env(ctx->env, "USER"));
	if (ctx)
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			ft_printf("\033[36m%s ", cwd);
			s_free(ctx, cwd);
		}
	}
	ft_printf("\033[33m%s \033[0m\n", "✗");
}
