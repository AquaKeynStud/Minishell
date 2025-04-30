/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:56:52 by arocca            #+#    #+#             */
/*   Updated: 2025/04/30 21:23:45 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <errno.h>
#include <stdio.h>
#include "minishell.h"

int exec_err(t_ctx *ctx, bool is_cmd)
{
	ctx->status = 1;
	if (is_cmd)
	{
		if (errno == ENOENT)
			ctx->status = 127;
		if (errno == EACCES || errno == EISDIR)
			ctx->status = 126;
	}
	return (ctx->status);
}

int	exit_with_code(t_ctx *ctx, int code)
{
	if (code >= 0)
		ctx->status = code;
	return (ctx->status);
}
