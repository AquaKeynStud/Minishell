/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/27 15:50:06 by arocca           ###   ########.fr       */
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
	ctx->status = 0;
	ctx->env = init_env(envp);
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
	return ;
}

static void	command_handler(t_ctx *ctx, char *cmd)
{
	t_ast	*ast;
	t_token	*tokens;

	tokens = tokenize(cmd);
	ast = parse_input(tokens);
	execute_ast(ctx, ast);
	free_tokens(&tokens);
	free_ast(ast);
	ast = NULL;
}

static void	get_input_loop(t_ctx *ctx)
{
	char	*input;

	while (1)
	{
		input = readline("minishell => ");
		if (!input)
			break ;
		if (*input) // On ajoute la chaine a l'historique si elle n'est pas vide
			add_history(input);
		ft_trim(&input, " \t"); // On enleve les espaces au début et à la fin
		command_handler(ctx, input);
		free(input);
	}
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	(void)argv;
	init_context(&ctx, envp); // Remplit la structure contexte
	sig_init();
	get_input_loop(&ctx); // Lance la détection des inputs avec readline
	secure_exit(&ctx, 0);
	return (0);
}
