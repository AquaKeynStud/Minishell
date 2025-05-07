/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:07:55 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/07 12:09:39 by abouclie         ###   ########.fr       */
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

void	check_n_flags(char **args, int *i, bool *newline)
{
	while (args[*i] && is_dash_n_flag(args[*i]))
	{
		*newline = false;
		(*i)++;
	}
}

void	print_echo_args(char **args, int i)
{
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
}

int	ft_echo(char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;
	if (!args[1])
	{
		ft_printf("\n");
		free(args);
		return (EXIT_SUCCESS);
	}
	check_n_flags(args, &i, &newline);
	print_echo_args(args, i);
	if (newline)
		ft_printf("\n");
	free(args);
	return (EXIT_SUCCESS);
}
