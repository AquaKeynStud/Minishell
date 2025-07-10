/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/11 00:06:41 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

/* -- Includes -- */
# include "libft.h"
# include <stdio.h>
# include <stdbool.h>

/* -- Structures -- */
typedef struct s_ctx	t_ctx;

typedef struct s_env
{
	char			*key;			// ex: "PATH"
	char			*value;			// ex: "/usr/bin:/bin"
	struct s_env	*next;
}				t_env;

/* -- Functions -- */
int		ft_echo(t_ctx *ctx, char **args);
int		double_dot(t_ctx *ctx, char **args);
int		ft_pwd(t_ctx *ctx, char **args, void *env);
int		ft_cd(t_ctx *ctx, char **args, t_env *env);
int		ft_unset(t_ctx *ctx, char **args, t_env *env);
int		ft_export(t_ctx *ctx, char **args, t_env **env);
int		ft_env(t_ctx *ctx, t_env *env, int argc, char **args);
int		ft_exit(t_ctx *ctx, int argc, char **args);

void	free_env(t_ctx *ctx, t_env **env);
int		count_args(char **args);
t_env	*copy_env_list(t_ctx *ctx, t_env *env);
void	print_sorted_env(t_ctx *ctx, t_env *env);
char	*check_env(t_env *env, char *req);
int		is_valid_key(char *key, char *arg);
char	*get_working_dir(char *cmd_request);
char	*get_from_env(t_env *env, const char *key);
void	append_env_node(t_env **env, t_env *new_node);
int		update_env(t_ctx *ctx, t_env *env, char *key, char *value);
t_env	*init_env(t_ctx *ctx, char **args, char **envp);
t_env	*create_env_node(t_ctx *ctx, const char *key, const char *value);
void	add_or_update_env(t_ctx *ctx, t_env **env, char *key, char *value);

#endif