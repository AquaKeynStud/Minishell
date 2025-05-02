/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/02 12:56:06 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

/* -- Includes -- */
# include "libft.h"
# include <stdio.h>
# include <stdbool.h>

/* -- Structures -- */
typedef struct s_env
{
	char			*key;			// ex: "PATH"
	char			*value;		// ex: "/usr/bin:/bin"
	struct s_env	*next;
}				t_env;

/* -- Functions -- */
int		ft_pwd(void);
int		ft_echo(char **args);
int		ft_cd(char **args, t_env *env);
int		ft_exit(int argc, char **args);
int		ft_unset(char **args, t_env *env);
int		ft_export(char **args, t_env **env);
int		ft_env(t_env *env, int argc, char **args);

void	free_env(t_env **env);
t_env	*init_env(char **envp);

int		count_args(char **args);
int		is_valid_key(char *key);
char	*get_current_dir(void);
char	*get_from_env(t_env *env, const char *key);
void	print_sorted_env(t_env *env);
void	append_env_node(t_env **env, t_env *new_node);
void	append_env_node(t_env **env, t_env *new_node);
t_env	*copy_env_list(t_env *env);
t_env	*create_env_node(const char *key, const char *value);
t_env	*create_env_node(const char *key, const char *value);

#endif