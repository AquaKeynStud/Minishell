/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interractive_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:03:44 by arocca            #+#    #+#             */
/*   Updated: 2025/05/15 14:52:50 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <signal.h>
#include "parsing.h"
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
		exit(ctx->status);
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

static int	here_doc_non_interractive(const char *limiter)
{
	char	*line;
	int		pipefd[2];

	if (pipe(pipefd) < 0)
		return (-1);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break;
		size_t len = ft_strlen(line);
		if (len && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (!ft_strcmp(line, limiter))
		{
			free(line);
			break;
		}
		ft_dprintf(pipefd[1], "%s\n", line);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

static int	here_doc(const char *limiter)
{
	char	*line;
	int		pipefd[2];

	if (isatty(STDIN_FILENO))
		return (here_doc_non_interractive(limiter));
	if (pipe(pipefd) < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strcmp(line, limiter))
		{
			free(line);
			break ;
		}
		ft_dprintf(pipefd[1], "%s\n", line);
		free(line);
	}
	close(pipefd[1]);
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
			fd = here_doc(ast->childs[0]->value);
			if (fd < 0)
				return (perr("heredoc", 1), 0);
			ast->fd = fd;
			register_fd(&ctx->fds, fd);
		}
		if (!get_redir(ctx, ast->childs[1]))
			return (0);
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/05/15 14:51:23 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include <stdio.h>
#include "lexing.h"
#include <stdlib.h>
#include "parsing.h"
#include <sys/stat.h>
#include "minishell.h"
#include "sigaction.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	init_context(t_ctx *ctx, char **argv, char **envp)
{
	struct stat	st;

	ctx->fds = NULL;
	ctx->ast = NULL;
	ctx->status = 0;
	ctx->env = init_env(argv, envp);
	ctx->tokens = NULL;
	ctx->has_found_err = false;
	ctx->err_in_tokens = false;
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
	if (!stat("/proc/self", &st))
		ctx->uid = ft_itoa(st.st_uid);
	return ;
}

static void	command_handler(t_ctx *ctx, char *cmd)
{
	t_ast	*ast;
	t_token	*tokens;

	tokens = tokenize(ctx, cmd, false);
	ast = parse_input(ctx, tokens);
	if (!get_redir(ctx, ast))
		return ;
	if (!has_bonus_err(ctx, tokens))
		return ;
	ctx->ast = ast;
	ctx->tokens = tokens;
	ctx->input = cmd;
	execute_ast(ctx, ast);
	free_tokens(&tokens);
	free_ast(ast);
	ast = NULL;
	ctx->has_found_err = false;
	ctx->err_in_tokens = false;
}

static void	get_input_loop(t_ctx *ctx)
{
	char	*input;

	while (1)
	{
		input = readline("minishell => ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		ft_trim(&input, " \t");
		command_handler(ctx, input);
		free(input);
		input = NULL;
	}
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	init_context(&ctx, argv, envp);
	sig_init();
	get_input_loop(&ctx);
	secure_exit(&ctx);
	return (0);
}
