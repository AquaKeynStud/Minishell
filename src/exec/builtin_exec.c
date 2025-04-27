/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:08:42 by arocca            #+#    #+#             */
/*   Updated: 2025/04/27 15:47:23 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

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
	if (!args)
	{
		ft_dprintf(2, "minishell: builtin: Invalid arguments\n");
		return (1);
	}
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
