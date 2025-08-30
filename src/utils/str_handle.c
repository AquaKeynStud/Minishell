/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:31:20 by arocca            #+#    #+#             */
/*   Updated: 2025/07/13 10:30:57 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "minishell.h"

void	sort_char_table(char **arr)
{
	int		i;
	int		j;
	int		len;
	char	*tmp;

	i = 0;
	len = 0;
	while (arr[len])
		len++;
	while (i < len)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

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

void	print_status(t_ctx *ctx)
{
	char	*cwd;

	if (ctx && ctx->status == 0)
		ft_printf("\033[0m\033[1m\033[32m%s  \033[0m", "➜");
	else
		ft_printf("\033[1m\033[31m%s  \033[0m", "➜");
	if (ctx && get_from_env(ctx->env, "USER"))
		ft_printf("\033[35m\033[1m[%s] ", get_from_env(ctx->env, "USER"));
	if (ctx && get_from_env(ctx->env, "SHLVL"))
		ft_printf("\033[31m\033[1m-%s- ", get_from_env(ctx->env, "SHLVL"));
	cwd = s_save(ctx, getcwd(NULL, 0));
	if (cwd)
	{
		ft_printf("\033[36m\033[1m%s \033[0m", cwd);
		s_free(ctx, cwd);
	}
	ft_printf("\033[33m\033[1m%s \033[0m\n", "✗");
}
