/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:49:56 by arocca            #+#    #+#             */
/*   Updated: 2025/04/27 11:56:52 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include "libft.h"
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

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*node;
	char	**split;
	t_env	*env_list;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], '='); // "VAR=VALUE"
		node = (t_env *)s_malloc(sizeof(t_env));
		node->key = ft_strdup(split[0]);
		if (split[1])
			node->value = ft_strdup(split[1]);
		else
			node->value = ft_strdup(""); // ou NULL
		node->next = env_list;
		env_list = node;
		free_split(split); // fonction  coder
		i++;
	}
	return (env_list);
}
