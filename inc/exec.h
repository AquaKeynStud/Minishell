/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:53:39 by arocca            #+#    #+#             */
/*   Updated: 2025/09/04 08:18:42 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

/* -- Includes -- */
# include "parsing.h"
# include "minishell.h"

/* -- Error - Functions -- */
bool	syntax_err(t_ctx *ctx, t_ast *ast);
int		execve_err(t_ctx *ctx, char **value);
int		exit_with_code(t_ctx *ctx, int code);
bool	has_one_redir(t_ctx *ctx, t_ast *ast);
int		redir_err(t_ctx *ctx, t_ast *ast, int exit_code);

/* -- Exec - Functions -- */
int		count_args(char **args);
void	sort_char_table(char **arr);
int		check_hd(t_ctx *ctx, t_ast *ast);
char	*get_working_dir(char *cmd_request);
int		pid_verification(t_ctx *ctx, t_ast *node);

char	**env_to_envp(t_ctx *ctx, t_env *env);
char	**ast_to_argv(t_ctx *ctx, t_ast *node);
char	*get_path(t_ctx *ctx, char *cmd, t_env *env);

int		execute_ast(t_ctx *ctx, t_ast *node);
int		exec_command(t_ctx *ctx, t_ast *ast);
int		exec_builtin(t_ctx *ctx, char **args, t_env *env);

/* -- Bonus - Functions -- */
int		exec_operators(t_ctx *ctx, t_ast *ast);
int		exec_subshell(t_ctx *ctx, t_ast *subtree);

/* -- Expand - Functions -- */
void	replace_char(t_ast *node);
void	sort_char_table(char **arr);
void	glob_ast(t_ctx *ctx, t_ast *node);
void	merge_ast(t_ctx *ctx, t_ast *node);
void	merge_redir(t_ctx *ctx, t_ast *node);
void	expand_args(t_ctx *ctx, t_ast *node);
void	globbing_all(t_ctx *ctx, t_ast *node);
void	check_command(t_ctx *ctx, t_ast *ast);
t_ast	*expand_childs(t_ctx *ctx, t_ast *node);
void	remove_wlcd(t_ctx *ctx, t_token *tokens);
void	split_ifs(t_ctx *ctx, t_ast *parent, t_ast *ast, int index);

#endif