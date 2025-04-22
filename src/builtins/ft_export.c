/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:32:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 14:28:51 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

static int	env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
		i++;
		env = env->next;
	}
}

static void	sort()
{
	
}

static void	print_sorted_env(t_env *env)
{
	int	i;
	int	size;
	t_env	**env_array;
	t_env	*tmp;

	i = 0;
	size = env_size(env);
	env_array = malloc(sizeof(t_env *) * size);
	tmp = env;
	while (tmp)
	{
		env_array[i++] = tmp;
		tmp = tmp->next;
	}
	sort();
}

int	ft_export(char **args, t_env *env)
{
	if (!args[1])
	{
		print_sorted_env(env);
		return (0);
	}
	else if (args[1])
	{
		return (0);
	}
	return (0);
}

// ------------ FONCTIONS D'UTILS POUR TEST --------------------
t_env *create_env_node(char *key, char *value)
{
	t_env *node = malloc(sizeof(t_env));
	node->key = ft_strdup(key);
	node->value = value ? ft_strdup(value) : NULL;
	node->next = NULL;
	return node;
}

void append_env_node(t_env **env, t_env *new_node)
{
	if (!*env)
	{
		*env = new_node;
		return;
	}
	t_env *tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

void free_env(t_env *env)
{
	t_env *tmp;
	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

// ------------ MAIN DE TEST --------------------
int	main(void)
{
	t_env *env = NULL;

	append_env_node(&env, create_env_node("PATH", "/usr/bin:/bin"));
	append_env_node(&env, create_env_node("USER", "student42"));
	append_env_node(&env, create_env_node("HOME", "/home/user"));
	append_env_node(&env, create_env_node("EMPTY", NULL));

	char *args[] = { "export", NULL };

	ft_export(args, env);

	free_env(env);
	return 0;
}