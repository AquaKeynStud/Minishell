/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:28:20 by arocca            #+#    #+#             */
/*   Updated: 2025/04/13 09:56:32 by arocca           ###   ########.fr       */
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
void	open_fd(t_fd **head, int fd);
void	close_fd(t_fd **head, int fd);
void	close_fd(t_fd **head, int fd);

#endif