/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:16:49 by arocca            #+#    #+#             */
/*   Updated: 2025/04/07 11:20:47 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "readline.h"
#include "sigaction.h"

/**
 * @brief	Handler du signal SIGINT (envoyé par Ctrl+C)
 * 
 * Cette fonction permet de gérer le signal SIGINT, envoyé au programme lors
 * de la combinaison de touches Ctrl+C.
 * Peut être ajouter un fd pour gérer la sortie ou l'affichage ?
 * 
 * @param signal	Le signal que va recevoir la fonction
 */
void	handle_sigint_sigquit(int signal)
{
	if (signal == SIGQUIT || signal != SIGINT) // Si c'est Ctrl+\, on ne fait rien. Sinon, c'est SIGINT
		return ;
	write(1, "\n", 1); // Write pour éviter les allocs
	rl_replace_line("", 0); // Vide la ligne de l'input
	rl_on_new_line(); // Passage à une nouvelle ligne
	rl_redisplay(); // Màj de l'affichage
}
