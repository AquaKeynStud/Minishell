/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:23:03 by arocca            #+#    #+#             */
/*   Updated: 2025/07/13 10:11:34 by arocca           ###   ########.fr       */
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

static int	exec_side_pipe(t_ctx *ctx, t_ast *ast, int fds[2], bool is_l_side)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		sig_set(SIG_DFL);
		if (is_l_side)
			dup2(fds[1], STDOUT_FILENO);
		else
			dup2(fds[0], STDIN_FILENO);
		close_fd(&ctx->fds, fds[0]);
		close_fd(&ctx->fds, fds[1]);
		ctx->status = execute_ast(ctx, ast);
		secure_exit(ctx);
	}
	else if (pid < 0)
		perror_code("pipe fork", -1);
	return (pid);
}

static int	exec_pipe(t_ctx *ctx, t_ast *ast)
{
	int	ret;
	int	fds[2];
	int	pid_left;
	int	pid_right;

	ret = pipe(fds);
	if (ret != 0)
		return (exit_with_code(ctx, s_exec_exit(ctx->status)));
	register_fd(&ctx->fds, fds[0]);
	register_fd(&ctx->fds, fds[1]);
	sig_set(SIG_IGN);
	pid_left = exec_side_pipe(ctx, ast->childs[0], fds, true);
	pid_right = exec_side_pipe(ctx, ast->childs[1], fds, false);
	close_fd(&ctx->fds, fds[0]);
	close_fd(&ctx->fds, fds[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &ctx->status, 0);
	sig_init();
	return (s_exec_exit(ctx->status));
}

static int	exec_redir(t_ctx *ctx, t_ast *ast)
{
	int	fd;
	int	pid;

	fd = pid_verification(ctx, ast);
	if (fd < 0)
		return (ctx->status);
	pid = fork();
	if (pid < 0)
		return (redir_err(ctx, ast, 0));
	else if (pid == 0)
	{
		sig_set(SIG_DFL);
		if (ast->value[0] == '<')
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		ctx->status = execute_ast(ctx, ast->childs[1]);
		secure_exit(ctx);
	}
	close_fd(&ctx->fds, fd);
	waitpid(pid, &ctx->status, 0);
	sig_init();
	return (s_exec_exit(ctx->status));
}

static int	exec_command(t_ctx *ctx, t_ast *ast)
{
	int		pid;
	char	*path;
	char	**envp;
	char	**args;

	envp = env_to_envp(ctx, ctx->env);
	args = ast_to_argv(ctx, ast);
	path = get_path(ctx, ast->value, ctx->env);
	if (!path || !args || !envp)
		return (execve_err(ctx, args));
	sig_set(SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror_code("command fork", 2));
	if (pid == 0)
	{
		sig_set(SIG_DFL);
		close_unregistered_fds(ctx);
		execve(path, args, envp);
		execve_err(ctx, args);
		secure_exit(ctx);
	}
	waitpid(pid, &ctx->status, 0);
	sig_init();
	return (s_exec_exit(ctx->status));
}

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

int	execute_ast(t_ctx *ctx, t_ast *ast)
{
	if (!ast)
		return (ctx->status);
	if (ast->type == AST_PIPE)
		ctx->status = exec_pipe(ctx, ast);
	else if (ast->type == AST_REDIR && ast->fd == -1)
		ctx->status = exec_redir(ctx, ast);
	else if (ast->type == AST_AND || ast->type == AST_OR)
		ctx->status = exec_operators(ctx, ast);
	else if (ast->type == AST_SUB)
		ctx->status = exec_subshell(ctx, ast->childs[0]);
	else if (ast->type == AST_COMMAND && ast->value)
	{
		expand_childs(ctx, ast);
		// if (!syntax_error(ctx, ctx->tokens))
		// 	return (ctx->status);
		if (ast->value && !ft_strcmp(ast->value, "!"))
			ctx->status = 1;
		else if (ast->value && is_builtin(ast->value))
			ctx->status = exec_builtin(ctx, ast_to_argv(ctx, ast), ctx->env);
		else
			ctx->status = exec_command(ctx, ast);
	}
	return (ctx->status);
}
