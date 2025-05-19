/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:16:49 by arocca            #+#    #+#             */
/*   Updated: 2025/05/16 13:43:21 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "sigaction.h"
#include "minishell.h"
#include <readline/readline.h>

void	handle_sigint_sigquit(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	if (COLOR)
		print_status(NULL);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
