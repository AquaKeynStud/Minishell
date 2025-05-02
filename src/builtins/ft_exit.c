/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:13:26 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/02 18:39:02 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

static int	str_is_numeric(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
	{
		i++;
	}
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (2);
		i++;
	}
	return (0);
}

int	ft_exit(int argc, char **args)
{
	int	arg;

	if (argc > 2)
	{
		ft_dprintf(2, "exit: %s: too many arguments\n", args[1]);
		return (1);
	}
	if (argc == 2)
	{
		if (str_is_numeric(args[1]) == 2)
		{
			ft_dprintf(2, "exit: %s: numeric argument required\n", args[1]);
			exit(2);
		}
		else
		{
			arg = ft_atoi(args[1]) % 256;
			exit(arg);
		}
	}
	free(args);
	exit(0);
}
