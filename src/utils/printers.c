/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 19:49:52 by arocca            #+#    #+#             */
/*   Updated: 2025/04/22 17:02:01 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <unistd.h>

int	err(char *message) // Potentiellement ajouter un retour a la ligne si messages trop longs
{
	ssize_t	count;

	count = write(STDERR_FILENO, message, ft_strlen(message));
	if (count == -1)
		perror("write");
	return (count);
}

int	perr(const char *s, int	return_value)
{
	perror(s);
	return (return_value);
}