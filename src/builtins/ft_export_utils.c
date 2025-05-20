/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:47:13 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/15 01:14:18 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

t_env	*copy_env_list(t_env *env)
{
	t_env	*copy;

	copy = NULL;
	while (env)
	{
		append_env_node(&copy, create_env_node(env->key, env->value));
		env = env->next;
	}
	return (copy);
}

int	is_valid_key(char *key, char *arg)
{
	int	i;

	i = 0;
	if (key[0] == '-' && key[1])
	{
		ft_dprintf(2, "minishell: export: ");
		ft_dprintf(2, "%c%c: invalid option\n", key[0], key[1]);
		return (2);
	}
	if (ft_isdigit(key[i]))
	{
		ft_dprintf(2, "export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			ft_dprintf(2, "export: `%s': not a valid identifier\n", arg);
			return (1);
		}
		i++;
	}
	return (0);
}

t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	if (value && !new->value)
	{
		free(new->key);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

void	append_env_node(t_env **env, t_env *new_node)
{
	t_env	*current;

	if (!*env)
		*env = new_node;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}
