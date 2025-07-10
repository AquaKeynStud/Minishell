/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:07:55 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/10 19:13:35 by arocca           ###   ########.fr       */
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

static void	print_echo(char	*arg)
{
	bool	is_last_bslash;

	is_last_bslash = false;
	while (*arg)
	{
		if (*arg == '\\' && !is_last_bslash)
			is_last_bslash = true;
		else
		{
			is_last_bslash = false;
			ft_printf("%c", *arg);
		}
		arg++;
	}
}

bool	has_nl(char **args, int *i)
{
	bool	newline;

	newline = true;
	while (args[(*i)] && is_dash_n_flag(args[(*i)]))
	{
		newline = false;
		(*i)++;
	}
	return (newline);
}

int	ft_echo(t_ctx *ctx, char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = has_nl(args, &i);
	if (!args[1])
	{
		ft_printf("\n");
		s_free(ctx, args);
		return (EXIT_SUCCESS);
	}
	while (args[i])
	{
		print_echo(args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	s_free(ctx, args);
	return (EXIT_SUCCESS);
}
