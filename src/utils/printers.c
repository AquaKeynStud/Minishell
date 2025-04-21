/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:49:52 by arocca            #+#    #+#             */
/*   Updated: 2025/04/20 19:57:30 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	err(char *message) // Potentiellement ajouter un retour a la ligne si messages trop longs
{
	size_t	count;

	count = write(STDERR_FILENO, message, ft_strlen(message));
	if (count == -1)
		perror("write");
	return (count);
}