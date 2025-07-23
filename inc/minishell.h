/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:58:51 by arocca            #+#    #+#             */
/*   Updated: 2025/07/23 00:56:48 by abouclie         ###   ########.fr       */
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

typedef struct s_allocs
{
	void			*alloc;
	struct s_allocs	*next;
}			t_allocs;

typedef struct s_ctx
{
	char		*uid;
	t_env		*env;
	t_fd		*fds;
	t_ast		*ast;
	int			status;
	char		*input;
	t_token		*tokens;
	t_allocs	*allocs;
	int			stdin_fd;
	int			stdout_fd;
	bool		err_in_tokens;
	bool		has_found_err;
	bool		sigint_in_heredoc;
	bool		has_wildcard;
	bool		*is_quoted;
	int			index_wildcards;
}			t_ctx;

/* -- Macros -- */
# define TRUNC_FLAGS 577 // (O_CREAT | O_TRUNC | O_WRONLY)
# define APPEND_FLAGS 1089 // (O_CREAT | O_APPEND | O_WRONLY)

# ifndef COLOR
#  define COLOR false
# endif

/* -- File descriptor - Garbage collector -- */
void	close_all_fds(t_fd **head);
void	close_fd(t_fd **head, int fd);
int		register_fd(t_fd **head, int fd);
void	close_unregistered_fds(t_ctx *ctx);
int		open_fd(t_fd **head, const char *filepath, int flags, mode_t perms);

/* -- Allocations - Garbage collector -- */
void	free_garbage(t_allocs **head);
void	s_free(t_ctx *ctx, void *ptr);
void	*s_save(t_ctx *ctx, void *ptr);
void	s_delete(t_ctx *ctx, void *ptr);
void	*s_malloc(t_ctx *ctx, size_t size);
void	*s_realloc(t_ctx *ctx, void *ptr, size_t old_size, size_t new_size);

/* -- Other - Functions -- */
int		s_exec_exit(int status);
void	secure_exit(t_ctx *ctx);
void	print_status(t_ctx *ctx);
int		set_status(t_ctx *ctx, int value);
int		perror_code(const char *error, int exit_code);
char	*ft_strjoin_free(t_ctx *ctx, char *s1, char *s2);
char	*join_with_delim(t_ctx *ctx, char *s1, char *s2, char *delimiter);
void	double_free(t_ctx *ctx, void **ptr, size_t size_if_not_null_term);

#endif