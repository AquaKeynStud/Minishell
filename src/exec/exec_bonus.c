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

bool	syntax_err(t_ctx *ctx, t_ast *ast)
{
	int	i;

	if (ast->type == AST_REDIR && !ft_strcmp(ast->value, "<<"))
	{
		if (!ast->childs[0] || !ast->childs[1])
			return (true);
	}
	i = 0;
	while (i < ast->sub_count)
	{
		if (syntax_err(ctx, ast->childs[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	has_one_redir(t_ctx *ctx, t_ast *ast)
{
	char	*err;

	err = "ambiguous redirect";
	if (ast->sub_count > 2)
	{
		if (ast->sub_count && ast->childs)
			ft_dprintf(2, "minishell: %s: %s\n", ast->childs[0]->value, err);
		else
			perror("minishell");
		ctx->status = 1;
		return (false);
	}
	return (true);
}

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
