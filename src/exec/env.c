/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:49:56 by arocca            #+#    #+#             */
/*   Updated: 2025/04/23 10:53:55 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "export")
		);
}

int	exec_builtin(char **args, t_env *env)
{
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(args, env));
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(args));
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(args, env));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(args, env));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env(env));
	if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(args));
	return (-1);
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*node;
	char 	**split;
	int	i;
	
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