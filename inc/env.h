/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/26 13:39:12 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

/* -- Includes -- */
# include "libft.h"
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
int		ft_env(t_env *env);
int		ft_echo(char **args);
int		ft_exit(char **args);
int		ft_cd(char **args, t_env *env);
int		ft_unset(char **args, t_env *env);
int		ft_export(char **args, t_env *env);

void	free_env(t_env **env);
t_env	*init_env(char **envp);
char	*get_from_env(t_env *env, const char *key);

void	free_split(char **split);

#endif