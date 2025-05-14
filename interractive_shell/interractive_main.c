/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interractive_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/05/14 16:42:41 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include <stdio.h>
#include "lexing.h"
#include <stdlib.h>
#include "parsing.h"
#include "minishell.h"
#include "sigaction.h"
#include <readline/history.h>
#include <readline/readline.h>

static int	here_doc(const char *limiter)
{
	char	*line;
	char	*prompt;
	int		pipefd[2];

	if (pipe(pipefd) < 0)
		return (-1);
	prompt = NULL;
	if (isatty(STDIN_FILENO))
		prompt = "> ";
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline(prompt);
		else
			line = get_next_line(STDIN_FILENO);
		if (!line)
			break;
		// supprimer le '\n' final quand on vient de get_next_line
		if (!isatty(STDIN_FILENO))
		{
			size_t len = ft_strlen(line);
			if (len && line[len - 1] == '\n')
				line[len - 1] = '\0';
		}
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

static void	init_context(t_ctx *ctx, char **envp)
{
	ctx->fds = NULL;
	ctx->ast = NULL;
	ctx->status = 0;
	ctx->env = init_env(envp);
	ctx->tokens = NULL;
	ctx->has_found_err = false;
	ctx->err_in_tokens = false;
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
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

static bool is_interactive(void)
{
	return isatty(STDIN_FILENO);
}

/* Renvoie NULL à l’EOF ou en cas d’erreur */
char *get_input(void)
{
	char *input;

	if (is_interactive())
	{
		/* mode REPL */
		input = readline("minishell => ");
	}
	else
	{
		/* mode batch : on lit une ligne brute depuis stdin */
		input = get_next_line(STDIN_FILENO);
		if (input)
		{
			/* supprimer le ‘\n’ final pour être cohérent avec readline() */
			size_t len = ft_strlen(input);
			if (len > 0 && input[len - 1] == '\n')
				input[len - 1] = '\0';
		}
	}
	return input;
}

static void get_input_loop(t_ctx *ctx)
{
	char *input;

	while (1)
	{
		input = get_input();
		if (!input)
			break;                 /* EOF ou erreur */
		if (*input)
			add_history(input);    /* pas d’historique en batch mais harmless */

		ft_trim(&input, " \t");
		command_handler(ctx, input);
		free(input);
	}

	/* nettoyage readline, ignorable en batch */
	if (is_interactive())
		rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	(void)argv;
	init_context(&ctx, envp);
	sig_init();
	get_input_loop(&ctx);
	secure_exit(&ctx);
	return (0);
}
