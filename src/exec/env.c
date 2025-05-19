/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:49:56 by arocca            #+#    #+#             */
/*   Updated: 2025/05/15 13:31:58 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include "libft.h"
#include <sys/stat.h>
#include "minishell.h"

char	*get_from_env(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static bool	fill_envp(char **envp, t_env *env)
{
	int		i;
	char	*key_val_str;
	t_env	*current;

	i = 0;
	current = env;
	while (current)
	{
		key_val_str = ft_strjoin(current->key, "=");
		if (!key_val_str)
			return (false);
		envp[i] = ft_strjoin(key_val_str, current->value);
		free(key_val_str);
		if (!envp[i])
			return (false);
		i++;
		current = current->next;
	}
	envp[i] = NULL;
	return (true);
}

char	**env_to_envp(t_env *env)
{
	int		i;
	t_env	*tmp;
	char	**envp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = (char **)s_malloc((i + 1) * sizeof(char *));
	if (!fill_envp(envp, env))
	{
		double_free((void **)envp, 0);
		return (NULL);
	}
	return (envp);
}

void	free_env(t_env **env)
{
	t_env	*next;
	t_env	*current;

	current = *env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}

static void	add_env(t_env **env, char **args)
{
	char	*shlvl;

	if (!get_from_env(*env, "PWD"))
		add_or_update_env(env, "PWD", get_working_dir("pwd"));
	if (get_from_env(*env, "SHLVL"))
	{
		shlvl = ft_itoa(ft_atoi(get_from_env(*env, "SHLVL")) + 1);
		add_or_update_env(env, "SHLVL", shlvl);
		free(shlvl);
	}
	else
		add_or_update_env(env, "SHLVL", "1");
	if (!get_from_env(*env, "_") && args[0])
		add_or_update_env(env, "_", args[0]);
}

t_env	*init_env(char **args, char **envp)
{
	t_env	*env_list;
	t_env	*node;
	char	*eq;

	env_list = NULL;
	while (*envp)
	{
		node = s_malloc(sizeof(t_env));
		eq = ft_strchr(*envp, '=');
		if (eq)
		{
			node->key = ft_substr(*envp, 0, eq - *envp);
			node->value = ft_strdup(eq + 1);
		}
		else
		{
			node->key = ft_strdup(*envp);
			node->value = NULL;
		}
		node->next = env_list;
		env_list = node;
		envp++;
	}
	add_env(&env_list, args);
	return (env_list);
}
