/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:49:52 by arocca            #+#    #+#             */
/*   Updated: 2025/07/03 10:15:43 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include "minishell.h"

void	print_status(t_ctx *ctx)
{
	char	*cwd;

	if (ctx && ctx->status == 0)
		ft_printf("\033[0m\033[1m\033[32m%s  ", "➜");
	else
		ft_printf("\033[1m\033[31m%s  ", "➜");
	if (ctx && get_from_env(ctx->env, "USER"))
		ft_printf("\033[35m[%s] ", get_from_env(ctx->env, "USER"));
	if (ctx)
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			ft_printf("\033[36m%s ", cwd);
			free(cwd);
		}
	}
	ft_printf("\033[33m%s \033[0m\n", "✗");
}

int	err(char *message)
{
	ssize_t	count;

	count = write(STDERR_FILENO, message, ft_strlen(message));
	if (count == -1)
		perror("write");
	return (count);
}

int	err_value(char *message, char *value)
{
	ssize_t	count;

	count = write(STDERR_FILENO, message, ft_strlen(message));
	count += write(STDERR_FILENO, value, ft_strlen(value));
	if (count == -1)
		perror("write");
	return (count);
}

int	perr(const char *s, int return_value)
{
	perror(s);
	return (return_value);
}
