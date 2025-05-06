/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:07:55 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/06 12:46:39 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdbool.h>
#include "minishell.h"
#include "sigaction.h"

bool	is_dash_n_flag(char *arg)
{
	if (!arg || !*arg || *arg != '-')
		return (false);
	while (*++arg)
	{
		if (*arg != 'n')
			return (false);
	}
	return (true);
}

int	ft_echo(char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;
	while (args[i] && is_dash_n_flag(args[i]))
	{
		newline = false;
		i++;
	}
	if (!args[1])
	{
		ft_printf("\n");
		return (EXIT_SUCCESS);
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	free(args);
	return (EXIT_SUCCESS);
}
