/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:39 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/29 23:38:40 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

static int	is_option(char **args)
{
	int		i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-')
		{
			ft_printf("unset: %s: no option allowed\n", args[i]);
			return (2);
		}
		i++;
	}
	return (0);
}

static void	remove_env_var(char *key, t_env **env)
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
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(char **args, t_env *env)
{
	int		i;
	int		exit_code;

	i = 1;
	exit_code = is_option(args);
	while (args[i])
	{
		remove_env_var(args[i], &env);
		i++;
	}
	return (exit_code);
}
