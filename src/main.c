/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/05/20 11:26:36 by abouclie         ###   ########.fr       */
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
	ctx->input = NULL;
	ctx->tokens = NULL;
	ctx->has_found_err = false;
	ctx->err_in_tokens = false;
	ctx->env = init_env(argv, envp);
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
	if (!stat("/proc/self", &st))
		ctx->uid = ft_itoa(st.st_uid);
	return ;
}

static void	destroy_command(t_ctx **ctx, t_token **tokens, t_ast **ast)
{
	free_tokens(tokens);
	free_ast(*ast);
	(*ctx)->ast = NULL;
	(*ctx)->tokens = NULL;
	(*ctx)->has_found_err = false;
	(*ctx)->err_in_tokens = false;
	return ;
}

static void	command_handler(t_ctx *ctx, char *cmd)
{
	t_ast	*ast;
	t_token	*tokens;

	tokens = tokenize(ctx, cmd, false);
	ast = parse_input(ctx, tokens);
	if (!get_redir(ctx, ast) || !has_bonus_err(ctx, tokens))
		return (destroy_command(&ctx, &tokens, &ast));
	ctx->ast = ast;
	ctx->tokens = tokens;
	execute_ast(ctx, ast);
	destroy_command(&ctx, &tokens, &ast);
}

static void	get_input_loop(t_ctx *ctx)
{
	char	*input;
	
	input = NULL;
	while (1)
	{
		if (COLOR)
		{
			print_status(ctx);
			input = readline(NULL);
		}
		else
			input = readline("➜  minishell ✗ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		ft_trim(&input, " \t");
		ctx->input = input;
		command_handler(ctx, input);
		free(input);
		ctx->input = NULL;
	}
}

/* int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	init_context(&ctx, argv, envp);
	sig_init();
	get_input_loop(&ctx);
	secure_exit(&ctx);
	return (0);
} */

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;

	if (!isatty(STDOUT_FILENO) || !isatty(STDERR_FILENO))
	{
		ft_dprintf(2, "minishell: output redirection or piping is not supported\n");
		exit(EXIT_FAILURE);
	}
	init_context(&ctx, argv, envp);
	sig_init();
	get_input_loop(&ctx);
	secure_exit(&ctx);
	return (0);
}
