/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/25 14:42:02 by arocca           ###   ########.fr       */
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
# include <readline/history.h>
# include <readline/readline.h>

static void	init_context(t_ctx *ctx, char **envp)
{
	ctx->env = init_env(envp);
	ctx->fds = NULL;
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
	ctx->status = 0;
	return ;
}

static void	get_input_loop(t_ctx *ctx)
{
	char	*input;
	t_token	*tokens;
	t_ast	*ast;

	while (1)
	{
		input = readline("minishell -> ");
		if (!input)
			break ;
		if (*input) // On ajoute la chaine a l'historique si elle n'est pas vide
			add_history(input);
		ft_trim(&input, " \t"); // On enleve les espaces au début et à la fin
		// printf("Nouvelle commande : [%s]\n", input); // Pour voir la commande enregistrée trimmée

		tokens = tokenize(input);
		ast = parse_input(tokens);
		execute_ast(ctx, ast);

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
	set_sigaction(SIGQUIT, handle_sigint_sigquit, "\0");
	set_sigaction(SIGINT, handle_sigint_sigquit, "1000000");
	get_input_loop(&ctx); // Lance la détection des inputs avec readline
	return (0);
}
