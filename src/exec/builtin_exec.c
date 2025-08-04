/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:08:42 by arocca            #+#    #+#             */
/*   Updated: 2025/07/13 11:34:42 by arocca           ###   ########.fr       */
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

static int	verif_env(t_ctx *ctx, char **args, t_env *env)
{
	if (get_from_env(env, "PATH"))
		return (ft_env(ctx, env, count_args(args), args));
	s_free(ctx, args);
	ft_dprintf(2, "minishell: ");
	ft_dprintf(2, "env: No such file or directory\n");
	return (127);
}

int	exec_builtin(t_ctx *ctx, char **args, t_env *env)
{
	if (!args)
	{
		ft_dprintf(2, "minishell: builtin: Invalid arguments\n");
		return (1);
	}
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(ctx, args, env));
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(ctx, args));
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd(ctx, args, env));
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(ctx, args, &env));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(ctx, args, env));
	if (!ft_strcmp(args[0], "env"))
		return (verif_env(ctx, args, env));
	if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(ctx, count_args(args), args));
	if (!ft_strcmp(args[0], ":"))
		return (double_dot(ctx, args));
	return (-1);
}
