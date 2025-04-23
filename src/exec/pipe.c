/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:23:03 by arocca            #+#    #+#             */
/*   Updated: 2025/04/23 12:21:41 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"
#include <stdbool.h>

void	exec_side_pipe(t_ctx *ctx, t_ast *node, int fds[2], bool is_left_pipe)
{
	int	pid;

	pid = fork();
    if (pid == 0)
    {
        if (is_left_pipe)
    		dup2(fds[1], STDOUT_FILENO);
		else
	    	dup2(fds[0], STDIN_FILENO);
        close_fd(&ctx->fds, fds[0]);
        close_fd(&ctx->fds, fds[1]);
        execute_ast(node, ctx);
        exit(ctx->status);
    }
}

int execute_pipe(t_ast *node, t_ctx *ctx)
{
	int	ret;
	int	pid;
	int	status;
	int	fds[2];

	ret = pipe(fds);
	if (ret != 0)
		return (1);
	open_fd(&ctx->fds, fds[0]);
	open_fd(&ctx->fds, fds[1]);
	exec_side_pipe(ctx, node->childs[0], fds, true);
	exec_side_pipe(ctx, node->childs[1], fds, false);
	close_fd(&ctx->fds, fds[0]);
	close_fd(&ctx->fds, fds[1]);
	waitpid(-1, &status, 0);
	waitpid(-1, &status, 0);
	return (s_exec_exit(status));
}
