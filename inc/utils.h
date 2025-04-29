/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2025/04/22 15:22:07 by arocca            #+#    #+#             */
/*   Updated: 2025/04/22 15:22:11 by arocca           ###   ########.fr       */
=======
/*   Created: 2025/04/13 09:28:20 by arocca            #+#    #+#             */
/*   Updated: 2025/04/20 19:59:41 by arocca           ###   ########.fr       */
>>>>>>> exec
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/* -- Includes -- */
# include <unistd.h>
# include <fcntl.h>

/* -- Structures -- */
typedef struct s_fd
{
    int fd;
    struct s_fd *next;
}			t_fd;

/* -- Functions -- */
int		err(char *message);

void	open_fd(t_fd **head, int fd);
void	close_fd(t_fd **head, int fd);
void	close_fd(t_fd **head, int fd);

void	*s_malloc(size_t size);
void	*s_calloc(size_t nmemb, size_t size);
void	*s_realloc(void *ptr, size_t old_size, size_t new_size);

#endif