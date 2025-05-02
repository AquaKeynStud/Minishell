/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:59:46 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/02 18:38:18 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "minishell.h"

static int	update_env(t_env *env, const char *key, const char *value)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (0);
		}
		env = env->next;
	}
	return (1);
}

static char	*get_cd_path(char **args, t_env *env)
{
	char	*path;

	path = args[1];
	if (!path)
	{
		path = get_from_env(env, "HOME");
		if (!path)
		{
			ft_dprintf(2, "cd: HOME is not set\n");
			return (NULL);
		}
	}
	return (path);
}

static int	perform_cd(char *path, char *oldpwd, t_env *env)
{
	char	*cwd;
	char	*perror_msg;

	if (chdir(path) != 0)
	{
		perror_msg = ft_strjoin("cd: ", path);
		perror(perror_msg);
		free(oldpwd);
		return (1);
	}
	cwd = get_current_dir();
	if (!cwd)
	{
		free(oldpwd);
		return (1);
	}
	update_env(env, "OLDPWD", oldpwd);
	update_env(env, "PWD", cwd);
	free(oldpwd);
	free(cwd);
	return (0);
}

int	ft_cd(char **args, t_env *env)
{
	char	*path;
	char	*oldpwd;
	int		result;
	int		arg_count;

	arg_count = count_args(args);
	if (arg_count > 2)
	{
		ft_dprintf(2, "cd: too many arguments\n");
		return (1);
	}
	path = get_cd_path(args, env);
	if (!path)
		return (1);
	oldpwd = get_current_dir();
	if (!oldpwd)
		return (1);
	result = perform_cd(path, oldpwd, env);
	free(args);
	return (result);
}
