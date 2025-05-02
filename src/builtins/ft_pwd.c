/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:24:54 by arocca            #+#    #+#             */
/*   Updated: 2025/05/02 18:24:20 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <limits.h>

static void	pwd_args(char **args, int	*situation)
{
	if (args[1] && *args[1] == '-' && (args[1][1] != 'L' && args[1][1] != 'P'))
	{
		ft_dprintf(2, "minishell: pwd: -%c: invalid option\n", args[1][1]);
		ft_dprintf(2, "pwd: usage: pwd [-LP]\n");
		(*situation) = -1;
	}
	else if (args[1] && !ft_strcmp(args[1], "-L"))
		(*situation) = 1;
	else
		(*situation) = 0;
	free(args);
	return ;
}

static int	logical_pwd(t_env *env)
{
	char	*pwd;

	pwd = get_from_env(env, "PWD");
	if (pwd)
	{
		ft_printf("%s\n", pwd);
		return (EXIT_SUCCESS);
	}
	perror("pwd");
	return (EXIT_FAILURE);
}

static int	physical_pwd(void)
{
	char	*cwd;

	cwd = get_working_dir("pwd");
	if (!cwd)
		return (EXIT_FAILURE);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}

int	ft_pwd(char **args, void *env)
{
	int	situation;

	pwd_args(args, &situation);
	if (situation < 0)
		return (EXIT_FAILURE);
	else if (situation > 0)
		return (logical_pwd(env));
	else
		return (physical_pwd());
}
