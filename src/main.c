/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/22 16:52:10 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "readline.h"
#include "sigaction.h"
#include "minishell.h"

void	init_context(t_ctx *ctx)
{
	ctx->env = init_env();
	ctx->fds = NULL;
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
	ctx->status = 0;
	return ;
}

int	main(void)
{
	t_ctx	ctx;

	init_context(&ctx); // Remplit la structure contexte
	set_sigaction(SIGQUIT, handle_sigint_sigquit, "\0");
	set_sigaction(SIGINT, handle_sigint_sigquit, "1000000");
	get_input_loop(); // Lance la détection des inputs avec readline
	return (0);
}
