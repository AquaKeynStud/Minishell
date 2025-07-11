/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:49:56 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 13:00:17 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include <sys/stat.h>
#include "minishell.h"

static bool	fill_envp(t_ctx *ctx, char **envp, t_env *env)
{
	int		i;
	char	*key_val_str;
	t_env	*current;

	i = 0;
	current = env;
	while (current)
	{
		key_val_str = s_save(ctx, ft_strjoin(current->key, "="));
		if (!key_val_str)
			return (false);
		envp[i] = s_save(ctx, ft_strjoin(key_val_str, current->value));
		s_free(ctx, key_val_str);
		if (!envp[i])
			return (false);
		i++;
		current = current->next;
	}
	envp[i] = NULL;
	return (true);
}

static void	add_env(t_ctx *ctx, t_env **env, char **args)
{
	char	*shlvl;

	if (!get_from_env(*env, "PWD"))
		add_or_update_env(ctx, env, "PWD", get_working_dir("pwd"));
	if (get_from_env(*env, "SHLVL"))
	{
		shlvl = s_save(ctx, ft_itoa(ft_atoi(get_from_env(*env, "SHLVL")) + 1));
		add_or_update_env(ctx, env, "SHLVL", shlvl);
		s_free(ctx, shlvl);
	}
	else
		add_or_update_env(ctx, env, "SHLVL", "1");
	if (!get_from_env(*env, "_") && args[0])
		add_or_update_env(ctx, env, "_", args[0]);
}

char	**env_to_envp(t_ctx *ctx, t_env *env)
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
	envp = (char **)s_malloc(ctx, (i + 1) * sizeof(char *));
	if (!fill_envp(ctx, envp, env))
	{
		double_free(ctx, (void **)envp, 0);
		return (NULL);
	}
	return (envp);
}

void	free_env(t_ctx *ctx, t_env **env)
{
	t_env	*next;
	t_env	*current;

	current = *env;
	while (current)
	{
		next = current->next;
		s_free(ctx, current->key);
		s_free(ctx, current->value);
		s_free(ctx, current);
		current = next;
	}
	*env = NULL;
}

t_env	*init_env(t_ctx *ctx, char **args, char **envp)
{
	t_env	*env_list;
	t_env	*node;
	char	*eq;

	env_list = NULL;
	while (*envp)
	{
		node = s_malloc(ctx, sizeof(t_env));
		eq = ft_strchr(*envp, '=');
		if (eq)
		{
			node->key = s_save(ctx, ft_substr(*envp, 0, eq - *envp));
			node->value = s_save(ctx, ft_strdup(eq + 1));
		}
		else
		{
			node->key = s_save(ctx, ft_strdup(*envp));
			node->value = NULL;
		}
		node->next = env_list;
		env_list = node;
		envp++;
	}
	add_env(ctx, &env_list, args);
	return (env_list);
}
