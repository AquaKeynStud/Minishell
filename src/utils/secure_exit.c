/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:15:31 by arocca            #+#    #+#             */
/*   Updated: 2025/05/14 12:42:49 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parsing.h"
#include "minishell.h"

void	secure_exit(t_ctx *ctx)
{
	free_env(&ctx->env);
	// if (ctx->ast)
	// 	free_ast(ctx->ast);
	// if (ctx->tokens)
	// 	free_tokens(&ctx->tokens);
	// free(ctx->input);
	close_all_fds(&ctx->fds);
	if (ctx->stdin_fd > 2)
		close(ctx->stdin_fd);
	if (ctx->stdout_fd > 2)
		close(ctx->stdout_fd);
	exit(ctx->status);
}

int	s_exec_exit(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
