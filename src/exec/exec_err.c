/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:56:52 by arocca            #+#    #+#             */
/*   Updated: 2025/04/30 10:18:37 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <errno.h>
#include <stdio.h>
#include "minishell.h"

int	execve_err(t_ctx *ctx, t_ast *ast)
{
	ctx->status = 1;
	if (errno == ENOENT)
		ctx->status = 127;
	else if (errno == EACCES || errno == EISDIR)
		ctx->status = 126;
	perror(ast->value);
	return (ctx->status);
}

int	exit_with_code(t_ctx *ctx, int code)
{
	if (code >= 0)
		ctx->status = code;
	return (ctx->status);
}

int	out_err(t_ctx *ctx, char *value, int code)
{
	ctx->status = exit_with_code(ctx, code);
	if (!value || !*value)
		value = "NA";
	ft_dprintf(2, "minishell : ");
	perror(value);
	return (ctx->status);
}
