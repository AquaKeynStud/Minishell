/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:08:42 by arocca            #+#    #+#             */
/*   Updated: 2025/05/13 18:32:18 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, ":")
	);
}

int	exec_builtin(t_ctx *ctx, char **args, t_env *env)
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
		return (ft_pwd(args, env));
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(args, &env));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(args, env));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env(env, count_args(args), args));
	if (!ft_strcmp(args[0], "exit"))
	{
		ft_exit(ctx, count_args(args), args);
		return (ctx->status);
	}
	if (!ft_strcmp(args[0], ":"))
		return (double_dot(args));
	return (-1);
}
