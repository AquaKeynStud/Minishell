/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:23:03 by arocca            #+#    #+#             */
/*   Updated: 2025/04/25 15:00:27 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "parsing.h"
#include <stdbool.h>
#include <sys/wait.h>
#include "minishell.h"

static void	exec_side_pipe(t_ctx *ctx, t_ast *node, int fds[2], bool is_l_side)
{
	int	pid;

	pid = fork();
	if (pid == 0) // On est dans le child process
	{
		if (is_l_side)
			dup2(fds[1], STDOUT_FILENO);
		else
			dup2(fds[0], STDIN_FILENO);
		close_fd(&ctx->fds, fds[0]);
		close_fd(&ctx->fds, fds[1]);
		execute_ast(ctx, node);
		exit(ctx->status);
	}
}

int exec_pipe(t_ctx *ctx, t_ast *node)
{
	int	ret;
	int	status;
	int	fds[2];

	ret = pipe(fds);
	if (ret != 0)
		return (1);
	register_fd(&ctx->fds, fds[0]);
	register_fd(&ctx->fds, fds[1]);
	exec_side_pipe(ctx, node->childs[0], fds, true);
	exec_side_pipe(ctx, node->childs[1], fds, false);
	close_fd(&ctx->fds, fds[0]);
	close_fd(&ctx->fds, fds[1]);
	waitpid(-1, &status, 0);
	waitpid(-1, &status, 0);
	return (s_exec_exit(status));
}

int	exec_redir(t_ctx *ctx, t_ast *node)
{
	int	fd;
	int	status;

	if (!ft_strcmp(node->value, ">"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, TRUNC_FLAGS, 0644);
	else if (!ft_strcmp(node->value, ">>"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, APPEND_FLAGS, 0644);
	else // if (!ft_strcmp(node->value, "<"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, O_RDONLY, 0);
	// else /* "<<" */
	// 	fd = here_doc(node->childs[0]->value);
	if (fd < 0)
		return (perr("redir", 1));
	if (node->value[0] == '<')
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	status = execute_ast(ctx, node->childs[1]);
	dup2(ctx->stdin_fd, STDIN_FILENO);
	dup2(ctx->stdout_fd, STDOUT_FILENO);
	close_fd(&ctx->fds, fd);
	return (status);
}

int	exec_command(t_ctx *ctx, t_ast *node)
{
	int		pid;
	char	*path;
	char	**envp;
	char	**args;
	int		wstatus;

	args = ast_to_argv(node->childs, node->sub_count);
	if (is_builtin(node->value))
		return (exec_builtin(args, ctx->env));
	pid = fork();
	if (pid == 0)
	{
		envp = env_to_envp(ctx->env);
		path = get_path(node->value, ctx->env);
		if (!envp || !args || !path)
		{
			err_value("minishell: ", node->value);
			err(": command not found\n"); // A modifier quand y'aura le dprintf (printf sur fd)
			return (127);
		}
		execve(path, args, envp);
		perr("execve", 1);
	}
	waitpid(pid, &wstatus, 0);
	return (s_exec_exit(wstatus));
}

int	execute_ast(t_ctx *ctx, t_ast *node)
{
	if (!node)
		return (ctx->status);
	if (node->type == AST_PIPE)
		ctx->status = exec_pipe(ctx, node);
	else if (node->type == AST_REDIR)
		ctx->status = exec_redir(ctx, node);
	else
		ctx->status = exec_command(ctx, node);
	return (ctx->status);
}
