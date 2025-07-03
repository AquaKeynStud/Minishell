/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:58:51 by arocca            #+#    #+#             */
/*   Updated: 2025/07/03 15:43:40 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* -- Includes -- */
# include "env.h"
# include <fcntl.h>
# include <unistd.h>

/* -- Structures -- */
typedef struct s_ast	t_ast;
typedef struct s_token	t_token;

typedef struct s_fd
{
	int			fd;
	struct s_fd	*next;
}			t_fd;

typedef struct s_ctx
{
	char	*uid;
	t_env	*env;
	t_fd	*fds;
	t_ast	*ast;
	int		status;
	char	*input;
	t_token	*tokens;
	int		stdin_fd;
	int		stdout_fd;
	bool	err_in_tokens;
	bool	has_found_err;
	bool	sigint_in_heredoc;
}			t_ctx;

/* -- Macros -- */
# define TRUNC_FLAGS 577 // (O_CREAT | O_TRUNC | O_WRONLY)
# define APPEND_FLAGS 1089 // (O_CREAT | O_APPEND | O_WRONLY)

# ifndef COLOR
#  define COLOR false
# endif

/* -- Functions -- */
int		err(char *message);
int		perr(const char *s, int return_value);
int		err_value(char *message, char *value);

void	print_status(t_ctx *ctx);

void	close_all_fds(t_fd **head);
void	close_fd(t_fd **head, int fd);
int		register_fd(t_fd **head, int fd);
int		open_fd(t_fd **head, const char *filepath, int flags, mode_t perms);

t_ctx	*set_ctx(t_ctx *ctx);
void	*s_malloc(size_t size);
void	*s_calloc(size_t nmemb, size_t size);
void	double_free(void **ptr, size_t size_if_not_null_term);
void	*s_realloc(void *ptr, size_t old_size, size_t new_size);

char	*ft_strjoin_free(char *s1, char *s2);
char	*join_with_delim(const char *s1, const char *s2, const char *delimiter);

int		s_exec_exit(int status);
void	secure_exit(t_ctx *ctx);

#endif