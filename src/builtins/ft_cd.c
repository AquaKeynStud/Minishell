/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:59:46 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/24 10:36:36 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

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
	return (1); // non trouvé
}

static char *get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
			env = env->next;
		}
		return (NULL); // pas trouvé
	}
	
int	ft_cd(char **args, t_env *env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	*path;

	path = args[1];
	if (args[2])
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	if (!path)
	path = get_env_value(env, "HOME");
	if (args[2])
	{
		ft_printf("cd: HOME is not set\n");
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd");
		return (1);
	}
	oldpwd = ft_strdup(cwd);
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd (after chdir)");
		free(oldpwd);
		return (1);
	}
	update_env(env, "OLDPWD", oldpwd);
	update_env(env, "PWD", cwd);
	free(oldpwd);
	return (0);
}
