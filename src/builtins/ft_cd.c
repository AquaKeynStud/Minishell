/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:00:00 by user              #+#    #+#             */
/*   Updated: 2025/05/14 10:10:06 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_env(t_env *env, char *req)
{
	char	*path;

	path = get_from_env(env, req);
	if (!path)
	{
		ft_dprintf(2, "cd: %s not set\n", req);
		return (NULL);
	}
	return (path);
}

static char	*ensure_target_dir(char *args, t_env *env)
{
	char	*path;

	if (args != NULL && args[1] && ((*args == '~' && args[1]) || !ft_strcmp(args, "-")))
	{
		if (*args == '~' && args[1] == '/')
		{
			path = join_with_delim(check_env(env, "HOME"), args + 2, "/");
		}
		else if (!ft_strcmp(args, "-"))
		{
			path = check_env(env, "OLDPWD");
			ft_printf("%s\n", path);
		}
		else
			path = args;
		return (path);
	}
	else if (args && (*args != '~' || args[1]) && ft_strcmp(args, "--"))
		path = args;
	else
		path = check_env(env, "HOME");
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

static int	process_cd(char *path, char *oldpwd, t_env *env)
{
	char	*newpwd;

	if (chdir(path))
	{
		ft_dprintf(2, "minishell: cd: ");
		perror(path);
		if (path[0] == '~' && path[1] == '/')
			free(path);
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
	if (path[0] == '~' && path[1] == '/')
		free(path);
	return (EXIT_SUCCESS);
}

int	ft_cd(char **args, t_env *env)
{
	char	*path;
	char	*oldpwd;	

	if (count_args(args) > 2 && ft_strcmp(args[1], "--"))
	{
		ft_dprintf(2, "cd: too many arguments\n");
		free(args);
		return (1);
	}
	path = ensure_target_dir(args[1], env);
	if (!path)
		return (1);
	if (path[0] == '-' && path[1])
	{
		ft_dprintf(2, "cd: %c%c: invalid option\n", path[0], path[1]);
		return (1);
	}
	oldpwd = ft_strdup(get_from_env(env, "PWD"));
	if (!oldpwd)
		oldpwd = get_working_dir("cd");
	free(args);
	return (process_cd(path, oldpwd, env));
}
