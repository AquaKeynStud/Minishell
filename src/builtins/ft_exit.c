/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:13:26 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/13 19:36:09 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

static int	str_is_numeric(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (2);
		i++;
	}
	return (0);
}

static int	check_exit_args(int argc, char **args)
{
	if (argc > 2 && str_is_numeric(args[1]) == 0)
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	if (argc > 1)
	{
		if (str_is_numeric(args[1]) == 2)
		{
			ft_dprintf(2, "minishell: exit: ");
			ft_dprintf(2, "%s: numeric argument required\n", args[1]);
			return (2);
		}
	}
	return (0);
}

void	handle_exit(int argc, char **args)
{
	long long	arg;
	int			error;

	if (argc > 1)
	{
		arg = ft_atoll(args[1], &error) % 256;
		if (error == 2)
		{
			ft_dprintf(2, "minishell: exit: ");
			ft_dprintf(2, "%s: numeric argument required\n", args[1]);
			exit(error);
		}
		exit(arg);
	}
	free(args);
	exit(0);
}

int	ft_exit(int argc, char **args)
{
	int	error;

	error = check_exit_args(argc, args);
	if (error)
	{
		if (error == 2)
			exit(2);
		return (1);
	}
	handle_exit(argc, args);
	return (0);
}
