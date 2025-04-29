/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:08:45 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 17:20:30 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "stddef.h"

int	is_builtin(char *cmd)
{
	return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo")
		|| !strcmp(cmd, "pwd") || !strcmp(cmd, "export")
		|| !strcmp(cmd, "unset") || !strcmp(cmd, "env")
		|| !strcmp(cmd, "exit"));
}

int	exec_builtin(char **args, t_env *env)
{
	if (!strcmp(args[0], "cd"))
		return (ft_cd(args, env));
	if (!strcmp(args[0], "echo"))
		return (ft_echo(args));
	if (!strcmp(args[0], "pwd"))
		return (ft_pwd());
	if (!strcmp(args[0], "export"))
		return (ft_export(args, env));
	if (!strcmp(args[0], "unset"))
		return (ft_unset(args, env));
	if (!strcmp(args[0], "env"))
		return (ft_env(env));
	if (!strcmp(args[0], "exit"))
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
