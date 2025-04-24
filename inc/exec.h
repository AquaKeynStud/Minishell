/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:53:39 by arocca            #+#    #+#             */
/*   Updated: 2025/04/24 11:16:51 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

/* -- Includes -- */
# include "parsing.h"
# include "minishell.h"

/* -- Functions -- */
int exec_pipe(t_ctx *ctx, t_ast *node);
int	exec_redir(t_ast *node, t_ctx *ctx);
int execute_ast(t_ctx *ctx, t_ast *node);
int	execute_command(t_ast *node, t_ctx *ctx);

int	is_builtin(char *cmd);
int	exec_builtin(char **args, t_env *env);

#endif