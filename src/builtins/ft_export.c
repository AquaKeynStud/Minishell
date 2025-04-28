/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:32:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/28 09:09:39 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

void	sort(t_env **env)
{
	t_env	*current;
	int		sorted;

	if (!env || !(*env))
		return ;
	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		current = *env;
		while (current && current->next)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_env_value(current, current->next);
				sorted = 0;
			}
			current = current->next;
		}
	}
}

static void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return ;
		}
		current = current->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return ; // a gérer
	new->key = ft_strdup(key);
	if (!new->key)
	{
		free(new);
		return ;  // Si l'allocation échoue, on libère et on sort
	}
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	if (value && !new->value)
	{
		free(new->key);
		free(new);
		return ;  // Si l'allocation de la valeur échoue, on libère et on sort
	}
	new->next = NULL;
	if (!*env)
		*env = new;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

int	ft_export(char **args, t_env **env)
{
	int		arg_count;
	int		i;
	char	**split;

	arg_count = count_args(args);
	if (arg_count == 1)
	{
		print_sorted_env(*env);
		return (0);
	}
	i = 1;
	while (i < arg_count)
	{
		if (ft_strchr(args[i], '='))
		{
			split = ft_split(args[i], '=');
			if (split && split[0])
			{
				if (split[1] && *split[1])
					add_or_update_env(env, split[0], split[1]);
				else
					add_or_update_env(env, split[0], NULL);
			}
			free_split(split); // fonction utilitaire pour free un char**
		}
		else
			add_or_update_env(env, args[i], NULL);
		i++;
	}
	return (0);
}
