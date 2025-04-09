/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 11:08:43 by arocca            #+#    #+#             */
/*   Updated: 2025/04/09 11:11:36 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/* -- Includes -- */
#include <stdlib.c>

/* -- Functions -- */
size_t	ft_strlen(const char *str);

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

#endif