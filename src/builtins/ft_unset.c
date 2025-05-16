/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:39 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/15 17:49:05 by arocca           ###   ########.fr       */
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
	if (!args)
		return (0);
	else if (!args[1])
	{
		free(args);
		return (0);
	}
	exit_code = is_option(args);
	while (args[i])
	{
		remove_env_var(args[i], &env);
		i++;
	}
	free(args);
	return (exit_code);
}
