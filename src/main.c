/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/05/13 18:45:59 by arocca           ###   ########.fr       */
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

static void	get_input_loop(t_ctx *ctx)
{
	char	*input;

	while (1)
	{
		// if (isatty(STDIN_FILENO))
		input = readline("minishell => ");
		// else
		// 	input = readline(NULL);
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
	(void)argv;
	init_context(&ctx, envp);
	sig_init();
	get_input_loop(&ctx);
	secure_exit(&ctx);
	return (0);
}
