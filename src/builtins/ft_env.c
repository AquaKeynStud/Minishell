/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:15:48 by abouclie          #+#    #+#             */
/*   Updated: 2025/08/06 09:05:45 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

int	update_env(t_ctx *ctx, t_env *env, char *key, char *value)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			s_free(ctx, env->value);
			env->value = value;
			return (0);
		}
		env = env->next;
	}
	return (1);
}

int	ft_env(t_ctx *ctx, t_env *env, int argc, char **args)
{
	int		i;

	i = 1;
	if (argc >= 2)
	{
		while (args[i])
		{
			if (args[i][0] == '-')
				ft_printf("env: '%s': no option allowed\n", args[i]);
			else
				ft_printf("env: '%s': no argument allowed\n", args[i]);
			i++;
		}
		s_free(ctx, args);
		return (127);
	}
	while (env)
	{
		if (env->value)
			ft_printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	s_free(ctx, args);
	return (0);
}
