/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:39 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/10 19:15:02 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "minishell.h"

static int	is_option(char **args)
{
	if (args[1][0] == '-' && args[1][1])
	{
		ft_dprintf(2, "minishell: unset: ");
		ft_printf("%c%c: invalid option\n", args[1][0], args[1][1]);
		return (2);
	}
	return (0);
}

static void	remove_env_var(t_ctx *ctx, char *key, t_env **env)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			s_free(ctx, tmp->key);
			s_free(ctx, tmp->value);
			s_free(ctx, tmp);
			printf("ctx->env: %p\n", (void *)ctx->env);
			for (t_env *e = ctx->env; e; e = e->next)
				printf("  %s=%s\n", e->key, e->value);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(t_ctx *ctx, char **args)
{
	int		i;
	int		exit_code;

	i = 1;
	if (!args)
		return (0);
	else if (!args[1])
	{
		s_free(ctx, args);
		return (0);
	}
	exit_code = is_option(args);
	while (args[i])
	{
		remove_env_var(ctx, args[i], &ctx->env);
		i++;
	}
	s_free(ctx, args);
	return (exit_code);
}
