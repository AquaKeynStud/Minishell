/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/04/22 17:09:43 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

// Fonction qui va permettre de sortir n'importe quand
void	secure_exit(t_ctx *ctx, unsigned char code)
{
	close_all_fds(&ctx->fds); // close de tous les fds ouverts
	exit(code);
}

void	*s_malloc(size_t size)
{
	void	*alloc;

	if (!size)
		exit(1); // Trouver un moyen de secure exit
	alloc = malloc(size);
	if (!alloc)
	{
		perror("malloc");
		exit(1);
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
		exit(1);
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
		perror("ft_realloc");
		exit(1);
	}
	return (alloc);
}
