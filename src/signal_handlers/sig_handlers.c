/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:16:49 by arocca            #+#    #+#             */
/*   Updated: 2025/05/01 22:39:10 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "sigaction.h"
#include <readline/readline.h>

void	handle_sigint_sigquit(int signal)
{
	(void)signal;
	write(1, "\n", 1); // Write pour éviter les allocs
	rl_replace_line("", 0); // Vide la ligne de l'input
	rl_on_new_line(); // Passage à une nouvelle ligne
	rl_redisplay(); // Màj de l'affichage
}
