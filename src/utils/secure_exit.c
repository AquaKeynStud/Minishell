/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:15:31 by arocca            #+#    #+#             */
/*   Updated: 2025/07/12 20:19:12 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <signal.h>
#include "parsing.h"
#include "minishell.h"

void	secure_exit(t_ctx *ctx)
{
	s_free(ctx, ctx->uid);
	free_env(ctx, &ctx->env);
	if (ctx->ast)
		free_ast(ctx, ctx->ast);
	if (ctx->tokens)
		free_tokens(ctx, &ctx->tokens);
	if (ctx->input)
		s_free(ctx, ctx->input);
	free_garbage(&ctx->allocs);
	close_all_fds(&ctx->fds);
	close_unregistered_fds(ctx);
	if (ctx->stdin_fd > 2)
		close(ctx->stdin_fd);
	if (ctx->stdout_fd > 2)
		close(ctx->stdout_fd);
	exit(ctx->status);
}

int	s_exec_exit(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ft_dprintf(2, "\n");
		return (128 + sig);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exit_with_code(t_ctx *ctx, int code)
{
	if (code >= 0)
		ctx->status = code;
	return (ctx->status);
}
