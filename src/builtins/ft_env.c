/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:15:48 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/29 09:47:43 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	ft_env(t_env *env, int argc, char **args)
{
	char	*perror_msg;
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
	return (0);
}
