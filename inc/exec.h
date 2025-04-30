/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:53:39 by arocca            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/29 19:43:41 by arocca           ###   ########.fr       */
=======
/*   Updated: 2025/04/30 10:43:50 by arocca           ###   ########.fr       */
>>>>>>> exec
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

/* -- Includes -- */
# include "parsing.h"
# include "minishell.h"

/* -- Functions -- */
int		exec_pipe(t_ctx *ctx, t_ast *node);
int		exec_redir(t_ctx *ctx, t_ast *node);
int		execute_ast(t_ctx *ctx, t_ast *node);
int		exec_command(t_ctx *ctx, t_ast *node);

int		is_builtin(char *cmd);
int		exec_builtin(char **args, t_env *env);

int		free_cmd(char *path, char **args, char **envp, int exit_code);

void	exec_side_pipe(t_ctx *ctx, t_ast *node, int fds[2], bool is_l_side);

int		execve_err(t_ctx *ctx, t_ast *ast);

char	**env_to_envp(t_env *env);
char	**ast_to_argv(t_ctx *ctx, t_ast *node);
char	*get_path(const char *cmd, t_env *env);

int		get_redir(t_ctx *ctx, t_ast *ast);

int		exit_with_code(t_ctx *ctx, int code);
int		out_err(t_ctx *ctx, char *value, int code);
int		exec_err(t_ctx *ctx, char *value, int code);

#endif