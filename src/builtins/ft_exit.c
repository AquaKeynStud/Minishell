/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:13:26 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/29 14:15:48 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

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
		ft_printf("exit: too many arguments\n", args[1]);
		return (1);
	}
	if (argc == 2)
	{
		if (str_is_numeric(args[1]) == 2)
		{
			ft_printf("exit: %s: numeric argument required\n", args[1]);
			exit(2);
		}
		else
		{
			arg = ft_atoi(args[1]) % 256;
			exit(arg);
		}
	}
	exit(0);
}
