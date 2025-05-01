/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:56:52 by arocca            #+#    #+#             */
/*   Updated: 2025/05/01 10:35:39 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <errno.h>
#include <stdio.h>
#include "minishell.h"

int	execve_err(t_ctx *ctx, char **value)
{
	ctx->status = 1;
	if (errno == ENOENT)
	{
		ctx->status = 127;
		if (value && *value)
			ft_dprintf(2, "minishell: %s: command not found\n", *value);
		else
			ft_dprintf(2, "minishell: command not found\n");
	}
	else
	{
		if (errno == EACCES || errno == EISDIR)
			ctx->status = 126;
		if (value && *value)
			perror(*value);
		else
			perror("minishell");
	}
	return (ctx->status);
}

int	exit_with_code(t_ctx *ctx, int code)
{
	if (code >= 0)
		ctx->status = code;
	return (ctx->status);
}
