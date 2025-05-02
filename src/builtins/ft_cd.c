/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:00:00 by user              #+#    #+#             */
/*   Updated: 2025/05/02 18:21:26 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ensure_target_dir(char **args, t_env *env)
{
	char	*path;

	if (args[1])
		path = args[1];
	else
	{
		path = get_from_env(env, "HOME");
		if (!path)
		{
			ft_dprintf(2, "cd: HOME not set\n");
			return (NULL);
		}
	}
	return (path);
}

char	*get_working_dir(char *cmd_request)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_dprintf(2, "%s: error retrieving current directory: ", cmd_request);
		perror("getcwd: cannot access parent directories");
		return (NULL);
	}
	return (cwd);
}

static int	update_env(t_env *env, char *key, char *value)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = value;
			return (0);
		}
		env = env->next;
	}
	return (1);
}

static int	process_cd(char *path, char *oldpwd, t_env *env)
{
	char	*newpwd;

	if (chdir(path))
	{
		ft_dprintf(2, "minishell: cd: ");
		perror(path);
		return (EXIT_FAILURE);
	}
	newpwd = get_working_dir("chdir");
	if (!newpwd)
	{
		if (path[0] == '/')
			newpwd = ft_strdup(path);
		else
			newpwd = join_with_delim((char *)oldpwd, (char *)path, "/");
	}
	if (oldpwd)
		update_env(env, "OLDPWD", oldpwd);
	if (newpwd)
		update_env(env, "PWD", newpwd);
	return (EXIT_SUCCESS);
}

int	ft_cd(char **args, t_env *env)
{
	char	*path;
	char	*oldpwd;	

	if (count_args(args) > 2)
	{
		ft_dprintf(2, "cd: too many arguments\n");
		free(args);
		return (1);
	}
	path = ensure_target_dir(args, env);
	if (!path)
		return (1);
	oldpwd = ft_strdup(get_from_env(env, "PWD"));
	if (!oldpwd)
		oldpwd = get_working_dir("cd");
	free(args);
	return (process_cd(path, oldpwd, env));
}
