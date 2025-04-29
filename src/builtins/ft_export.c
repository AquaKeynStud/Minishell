/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:32:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/29 23:52:07 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "minishell.h"

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

static void	add_or_update_env(t_env **env, char *key, char *value)
{
	char	*existing;
	t_env	*new_node;

	existing = get_from_env(*env, key);
	if (existing)
	{
		free(existing);
		if (value)
			existing = ft_strdup(value);
		else
			existing = NULL;
	}
	else
	{
		new_node = create_env_node(key, value);
		if (!new_node)
			return ;
		append_env_node(env, new_node);
	}
}

static void	process_env_arg(char *arg, t_env **env)
{
	char	**split;

	if (ft_strchr(arg, '='))
	{
		split = ft_split(arg, '=');
		if (split && split[0])
		{
			if (split[1] && *split[1])
				add_or_update_env(env, split[0], split[1]);
			else
				add_or_update_env(env, split[0], NULL);
		}
		double_free((void **)split, 0);
	}
	else
		add_or_update_env(env, arg, NULL);
}

int	ft_export(char **args, t_env **env)
{
	int	arg_count;
	int	i;

	arg_count = count_args(args);
	if (arg_count == 1)
	{
		print_sorted_env(*env);
		return (0);
	}
	i = 1;
	while (i < arg_count)
	{
		process_env_arg(args[i], env);
		i++;
	}
	return (0);
}
