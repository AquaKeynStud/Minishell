/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:13:26 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/02 10:22:21 by arocca           ###   ########.fr       */
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
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}

void	handle_exit_err(char *arg, char *format)
{
	ft_dprintf(2, "minishell: exit: ");
	if (arg && *arg)
		ft_dprintf(2, "%s: ", arg);
	ft_dprintf(2, "%s\n", format);
}

int	ft_exit(int argc, char **args)
{
	int	arg;

	if (argc > 2)
	{
		handle_exit_err(NULL, "too many arguments");
		return (1);
	}
	if (argc == 2)
	{
		if (str_is_numeric(args[1]))
		{
			handle_exit_err(args[1], "numeric argument required");
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
