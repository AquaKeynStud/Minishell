/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/24 14:48:25 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "readline.h"
#include "sigaction.h"
#include "minishell.h"

static void	init_context(t_ctx *ctx, char **envp)
{
	ctx->env = init_env(envp);
	ctx->fds = NULL;
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
	ctx->status = 0;
	return ;
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	(void)argv;
	init_context(&ctx, envp); // Remplit la structure contexte
	set_sigaction(SIGQUIT, handle_sigint_sigquit, "\0");
	set_sigaction(SIGINT, handle_sigint_sigquit, "1000000");
	get_input_loop(); // Lance la détection des inputs avec readline
	return (0);
}
