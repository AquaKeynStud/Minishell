/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:53:39 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 00:37:58 by arocca           ###   ########.fr       */
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
int		exec_builtin(t_ctx *ctx, char **args, t_env *env);

int		pid_verification(t_ctx *ctx, t_ast *node);
int		get_redir(t_ctx *ctx, t_ast *ast, t_token *tokens);

char	**ast_to_argv(t_ctx *ctx, t_ast *node);
char	**env_to_envp(t_ctx *ctx, t_env *env);
char	*get_path(t_ctx *ctx, const char *cmd, t_env *env);

int		execve_err(t_ctx *ctx, char **value);
int		exit_with_code(t_ctx *ctx, int code);
int		redir_err(t_ctx *ctx, t_ast *ast, int exit_code);

#endif