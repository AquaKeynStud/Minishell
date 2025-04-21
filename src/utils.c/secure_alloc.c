/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/04/20 19:16:55 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include "parsing.h"

// Fonction qui va permettre de sortir n'importe quand
void	secure_exit(unsigned char code) // Le cast en unsigned char va faire en sorte de moduler le code pour ne jamais dépasser 255
{
	// free ici la grande structure etc
	exit(code);
}

void	*s_malloc(size_t size)
{
	void	*alloc;

	if (!size)
		secure_exit(1);
	alloc = malloc(size);
	if (!alloc)
	{
		perror("malloc");
		secure_exit(1);
	}
	return (alloc);
}

void	*s_calloc(size_t nmemb, size_t size)
{
	void	*alloc;

	if (!nmemb || !size || nmemb > SIZE_MAX / size)
		return (NULL); // vérifier si c'est pas mieux d'exit
	alloc = ft_calloc(nmemb, size);
	if (!alloc)
	{
		perror("ft_calloc");
		secure_exit(1);
	}
	return (alloc);
}

void	*s_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*alloc;

	if (!new_size)
		return (NULL);
	alloc = ft_realloc(ptr, old_size, new_size);
	if (!alloc)
	{
		perror("realloc");
		secure_exit(1);
	}
	return (alloc);
}
