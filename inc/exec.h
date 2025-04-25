/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:53:39 by arocca            #+#    #+#             */
/*   Updated: 2025/04/25 14:59:21 by arocca           ###   ########.fr       */
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

char	**env_to_envp(t_env *env);
char	*get_path(const char *cmd, t_env *env);
char	**ast_to_argv(t_ast **childs, int sub_count);

#endif