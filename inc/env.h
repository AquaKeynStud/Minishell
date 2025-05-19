/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/19 13:09:16 by arocca           ###   ########.fr       */
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
int		ft_echo(char **args);
int		double_dot(char **args);
int		ft_pwd(char **args, void *env);
int		ft_cd(char **args, t_env *env);
int		ft_unset(char **args, t_env *env);
int		ft_export(char **args, t_env **env);
int		ft_env(t_env *env, int argc, char **args);
int		ft_exit(t_ctx *ctx, int argc, char **args);

void	free_env(t_env **env);
int		count_args(char **args);
t_env	*copy_env_list(t_env *env);
void	print_sorted_env(t_env *env);
char	*check_env(t_env *env, char *req);
int		is_valid_key(char *key, char *arg);
t_env	*init_env(char **args, char **envp);
char	*get_working_dir(char *cmd_request);
char	*ft_strjoin_free(char *s1, char *s2);
char	*get_from_env(t_env *env, const char *key);
void	append_env_node(t_env **env, t_env *new_node);
int		update_env(t_env *env, char *key, char *value);
t_env	*create_env_node(const char *key, const char *value);
void	add_or_update_env(t_env **env, const char *key, const char *value);

#endif