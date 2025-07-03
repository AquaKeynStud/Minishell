/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:16:49 by arocca            #+#    #+#             */
/*   Updated: 2025/07/03 10:03:49 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "sigaction.h"
#include "minishell.h"
#include <readline/readline.h>

void	handle_sigint(int signal)
{
	t_ctx	*ctx;

	(void)signal;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	ctx = set_ctx(NULL);
	ctx->status = 130;
	write(1, "\n", 1);
	if (COLOR)
		print_status(ctx);
}

void	handle_sigint_heredoc(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	secure_exit(set_ctx(NULL));
}