/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/07/13 10:33:13 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parsing.h"
#include <sys/wait.h>
#include "sigaction.h"

int	exec_operators(t_ctx *ctx, t_ast *ast)
{
	ctx->status = execute_ast(ctx, ast->childs[0]);
	if (ast->type == AST_AND && ctx->status == 0)
		ctx->status = execute_ast(ctx, ast->childs[1]);
	else if (ast->type == AST_OR && ctx->status != 0)
		ctx->status = execute_ast(ctx, ast->childs[1]);
	return (ctx->status);
}

int	exec_subshell(t_ctx *ctx, t_ast *subtree)
{
	int	pid;

	sig_set(SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror_code("subcommand fork", 1));
	else if (pid == 0)
	{
		sig_set(SIG_DFL);
		close_unregistered_fds(ctx);
		ctx->status = execute_ast(ctx, subtree);
		secure_exit(ctx);
	}
	waitpid(pid, &ctx->status, 0);
	sig_init();
	return (s_exec_exit(ctx->status));
}
