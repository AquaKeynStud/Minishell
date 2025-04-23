/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:28:20 by arocca            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/20 19:20:56 by arocca           ###   ########.fr       */
=======
/*   Updated: 2025/04/20 19:59:41 by arocca           ###   ########.fr       */
>>>>>>> parsing
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
int		perr(const char *s, int	return_value);

void	close_all_fds(t_fd **head);
void	open_fd(t_fd **head, int fd);
void	close_fd(t_fd **head, int fd);

void	*s_malloc(size_t size);
void	*s_calloc(size_t nmemb, size_t size);
void	*s_realloc(void *ptr, size_t old_size, size_t new_size);

int		s_exec_exit(int status);
void	secure_exit(t_ctx *ctx, unsigned char code);

#endif