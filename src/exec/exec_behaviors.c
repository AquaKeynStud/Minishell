/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_behaviors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:03:44 by arocca            #+#    #+#             */
/*   Updated: 2025/05/21 12:09:40 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <signal.h>
#include "parsing.h"
#include <sys/wait.h>
#include "minishell.h"
#include "sigaction.h"
#include <readline/history.h>
#include <readline/readline.h>

int	exec_side_pipe(t_ctx *ctx, t_ast *node, int fds[2], bool is_l_side)
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
		ctx->status = execute_ast(ctx, node);
		secure_exit(ctx);
	}
	else if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}

int	pid_verification(t_ctx *ctx, t_ast *node)
{
	int	fd;

	if (!ft_strcmp(node->value, ">"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, TRUNC_FLAGS, 0644);
	else if (!ft_strcmp(node->value, ">>"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, APPEND_FLAGS, 0644);
	else if (!ft_strcmp(node->value, "<"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, O_RDONLY, 0);
	else
		fd = node->fd;
	if (fd < 0)
		return (redir_err(ctx, node, -1));
	return (fd);
}

static pid_t	fork_heredoc(t_ctx *ctx, int pipefd[2], char *prompt, const char *limiter)
{
	pid_t	pid;
	char	*line;

	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		while (1)
		{
			line = readline(prompt);
			if (!line || !ft_strcmp(line, limiter))
			{
				if (line)
					free(line);
				break ;
			}
			ft_dprintf(pipefd[1], "%s\n", line);
			free(line);
		}
		close(pipefd[1]);
		secure_exit(ctx);
	}
	return (pid);
}

int	here_doc(t_ctx *ctx, const char *limiter)
{
	pid_t	pid;
	char	*prompt;
	int		pipefd[2];

	if (pipe(pipefd) < 0)
		return (-1);
	prompt = NULL;
	if (isatty(STDIN_FILENO))
		prompt = "> ";
	pid = fork_heredoc(ctx, pipefd, prompt, limiter);
	close(pipefd[1]);
	waitpid(pid, &ctx->status, 0);
	if (WIFSIGNALED(ctx->status) && WTERMSIG(ctx->status) == SIGINT)
	{
		close(pipefd[0]);
		ctx->status = 130;
		return (-1);
	}
	else if (WIFEXITED(ctx->status))
		ctx->status = WEXITSTATUS(ctx->status);
	return (pipefd[0]);
}

int	get_redir(t_ctx *ctx, t_ast *ast)
{
	int	fd;

	if (!ast)
		return (1);
	if (ast->type == AST_PIPE)
	{
		if (!get_redir(ctx, ast->childs[0]) || !get_redir(ctx, ast->childs[1]))
			return (0);
	}
	else if (ast->type == AST_REDIR)
	{
		if (ft_strcmp(ast->value, "<<") == 0)
		{
			fd = here_doc(ctx, ast->childs[0]->value);
			if (fd < 0)
				return (0);
			ast->fd = fd;
			register_fd(&ctx->fds, fd);
		}
		if (!get_redir(ctx, ast->childs[1]))
			return (0);
	}
	return (1);
}
