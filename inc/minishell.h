/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:58:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/24 11:12:37 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* -- Includes -- */
# include "env.h"
# include <fcntl.h>
# include <unistd.h>

/* -- Structures -- */
typedef struct s_fd
{
	int			fd;
	struct s_fd	*next;
}			t_fd;

typedef struct	s_ctx
{
	t_env	*env;		// pointeur vers ton tableau d’env
	t_fd	*fds;
	int		stdin_fd;
	int		stdout_fd;
	int		status;
}			t_ctx;

/* -- Macros -- */
# define TRUNC_FLAGS (O_CREAT | O_TRUNC | O_WRONLY)
# define APPEND_FLAGS (O_CREAT | O_APPEND | O_WRONLY)

/* -- Functions -- */
int		err(char *message);
int		perr(const char *s, int	return_value);

void	close_all_fds(t_fd **head);
void	close_fd(t_fd **head, int fd);
int		register_fd(t_fd **head, int fd);
int		open_fd(t_fd **head, const char *filepath, int flags, mode_t perms);

void	*s_malloc(size_t size);
void	*s_calloc(size_t nmemb, size_t size);
void	*s_realloc(void *ptr, size_t old_size, size_t new_size);

int		s_exec_exit(int status);
void	secure_exit(t_ctx *ctx, unsigned char code);

#endif