/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:23:03 by arocca            #+#    #+#             */
/*   Updated: 2025/04/30 11:50:08 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include <errno.h>
#include "parsing.h"
#include <stdbool.h>
#include <sys/wait.h>
#include "minishell.h"
#include "sigaction.h"

int	exec_pipe(t_ctx *ctx, t_ast *node)
{
	int	ret;
	int	fds[2];

	ret = pipe(fds);
	if (ret != 0)
		return (exit_with_code(ctx, s_exec_exit(ctx->status)));
	register_fd(&ctx->fds, fds[0]);
	register_fd(&ctx->fds, fds[1]);
	sig_set(SIG_IGN);
	exec_side_pipe(ctx, node->childs[0], fds, true);
	exec_side_pipe(ctx, node->childs[1], fds, false);
	close_fd(&ctx->fds, fds[0]);
	close_fd(&ctx->fds, fds[1]);
	waitpid(-1, &ctx->status, 0);
	waitpid(-1, &ctx->status, 0);
	sig_init();
	return (s_exec_exit(ctx->status));
}

int	exec_redir(t_ctx *ctx, t_ast *node)
{
	int	fd;

	if (!ft_strcmp(node->value, ">"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, TRUNC_FLAGS, 0644);
	else if (!ft_strcmp(node->value, ">>"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, APPEND_FLAGS, 0644);
	else if (!ft_strcmp(node->value, "<"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, O_RDONLY, 0);
	else if (!ft_strcmp(node->value, "<<"))
		fd = node->fd;
	if (fd < 0)
		return (out_err(ctx, node->childs[0]->value, errno));
	if (node->value[0] == '<')
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	ctx->status = execute_ast(ctx, node->childs[1]);
	dup2(ctx->stdin_fd, STDIN_FILENO);
	dup2(ctx->stdout_fd, STDOUT_FILENO);
	close_fd(&ctx->fds, fd);
	return (s_exec_exit(ctx->status));
}

int	exec_command(t_ctx *ctx, t_ast *node)
{
	int		pid;
	char	*path;
	char	**envp;
	char	**args;

	args = ast_to_argv(ctx, node);
	if (is_builtin(node->value))
		return (exec_builtin(args, ctx->env));
	envp = env_to_envp(ctx->env);
	path = get_path(node->value, ctx->env);
	if (!path || !args || !envp)
		return (free_cmd(path, args, envp, 127));
	sig_set(SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		sig_set(SIG_DFL);
		execve(path, args, envp);
		free_cmd(path, args, envp, execve_err(ctx, node));
		exit(ctx->status);
	}
	waitpid(pid, &ctx->status, 0);
	sig_init();
	return (free_cmd(path, args, envp, s_exec_exit(ctx->status)));
}

int	execute_ast(t_ctx *ctx, t_ast *node)
{
	if (!node)
		return (ctx->status);
	if (!get_redir(ctx, node))
		return (ctx->status);
	if (node->type == AST_PIPE)
		ctx->status = exec_pipe(ctx, node);
	else if (node->type == AST_REDIR)
		ctx->status = exec_redir(ctx, node);
	else if (node->type == AST_COMMAND && node->value)
		ctx->status = exec_command(ctx, node);
	return (ctx->status);
}
