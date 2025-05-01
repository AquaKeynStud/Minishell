/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:15:48 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/01 20:36:34 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

int	ft_env(t_env *env, int argc, char **args)
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
		return (1);
	}
	while (env)
	{
		if (env->value)
			ft_printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	free(args);
	return (0);
}
