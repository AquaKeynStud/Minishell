/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buitins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:11:12 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 13:32:07 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	ft_pwd()
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		ft_printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

int	ft_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			ft_printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

int	ft_unset(char **args, t_env *env)
{
	int		i;
	t_env	*prev;
	t_env	*tmp;
	
	i = 1;
	while (args[i])
	{
		prev = NULL;
		tmp = env;
		while (tmp) {
			if (!strcmp(tmp->key, args[i]))
			{
				if (prev)
					prev->next = tmp->next;
				else
					env = tmp->next;
				free(tmp->key);
				free(tmp->value);
				free(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (0);
}
int	ft_export(char **args, t_env *env)
{
	if (!args[1])
	{
		while (env)
		{
			ft_printf("declare -x %s=%s", env->key, env->value);
			env = env->next;
		}
	}
}


int	ft_exit(args)
{
	
}
