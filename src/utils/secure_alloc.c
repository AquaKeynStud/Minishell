/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/04/18 12:54:40 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include "parsing.h"

// Fonction qui va permettre de sortir n'importe quand
void	secure_exit(unsigned char code) // Le cast en unsigned char va faire en sorte de moduler le code pour ne jamais dépasser 255
{
	// free ici la grande structure etc
	close_all_fds(head); // Head non défini pour le moment, a stocker dans la structure
	exit(code)
}

void	*s_malloc(int size)
{
	void	*alloc;

	if (!size)
		secure_exit(1);
	alloc = malloc(size);
	if (!alloc)
		secure_exit(1);
}

void	*s_calloc(int size, int nbr)
{
	void	*alloc;

	if (!size || !nbr)
		secure_exit(1);
	alloc = ft_calloc(size, nbr);
	if (!alloc)
		secure_exit(1);
}