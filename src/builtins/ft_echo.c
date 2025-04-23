/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:07:55 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/23 10:19:56 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdbool.h>

// Fonction qui permet de savoir si l'argument passé correspond à un flag -n
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
	while (args[1] && is_dash_n_flag(args[i])) // Tant que les args sont des -n (ou -nnnn etc)
	{
		newline = false;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
