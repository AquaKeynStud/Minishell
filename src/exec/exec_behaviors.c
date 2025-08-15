/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_behaviors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:03:44 by arocca            #+#    #+#             */
/*   Updated: 2025/07/12 21:04:14 by arocca           ###   ########.fr       */
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

static void	exec_heredoc(char *prompt, const char *eof, int pipefd[2])
{
	char	*line;

	while (1)
	{
		line = readline(prompt);
		if (!line || !ft_strcmp(line, eof))
		{
			if (line)
				free(line);
			break ;
		}
		ft_dprintf(pipefd[1], "%s\n", line);
		free(line);
	}
}

static pid_t	fork_heredoc(t_ctx *ctx, int pipefd[2], char *prompt, char *eof)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		set_sigaction(SIGINT, handle_sigint_heredoc, "0000000");
		close(pipefd[0]);
		exec_heredoc(prompt, eof, pipefd);
		close_unregistered_fds(ctx);
		secure_exit(ctx);
	}
	return (pid);
}

static int	here_doc(t_ctx *ctx, char *eof)
{
	pid_t	pid;
	char	*prompt;
	int		pipefd[2];

	if (!eof || !*eof)
		return (-1);
	if (pipe(pipefd) < 0)
		return (-1);
	sig_set(SIG_IGN);
	prompt = NULL;
	if (isatty(STDIN_FILENO))
		prompt = "> ";
	pid = fork_heredoc(ctx, pipefd, prompt, eof);
	close(pipefd[1]);
	waitpid(pid, &ctx->status, 0);
	sig_init();
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

int	check_hd(t_ctx *ctx, t_ast *ast)
{
	int	fd;

	if (!ast)
		return (1);
	if (ast->type == AST_SUB)
		return (check_hd(ctx, ast->childs[0]));
	if (ast->type == AST_PIPE || ast->type == AST_AND || ast->type == AST_OR)
		return (check_hd(ctx, ast->childs[0]) && check_hd(ctx, ast->childs[1]));
	else if (ast->type == AST_REDIR)
	{
		if (!ft_strcmp(ast->value, "<<"))
		{
			if (!ast->childs[0]->value || !*ast->childs[0]->value)
				return (parsing_err(ctx, "newline", 2));
			fd = here_doc(ctx, ast->childs[0]->value);
			if (fd < 0)
				return (0);
			ast->fd = fd;
			register_fd(&ctx->fds, fd);
		}
		if (!check_hd(ctx, ast->childs[1]))
			return (0);
	}
	return (1);
}
