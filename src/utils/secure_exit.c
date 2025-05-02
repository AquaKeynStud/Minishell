/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:15:31 by arocca            #+#    #+#             */
/*   Updated: 2025/05/02 19:37:25 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

void	secure_exit(t_ctx *ctx)
{
	free_env(&ctx->env);
	close_all_fds(&ctx->fds);
	close(ctx->stdin_fd);
	close(ctx->stdout_fd);
	exit(ctx->status);
}

int	s_exec_exit(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
