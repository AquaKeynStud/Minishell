/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/28 11:56:32 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;

/* Prototypes */
int		ft_echo(char **args);

void	malloc_fail(void);
void	free_split(char **split);

/* ft_export */
int		ft_export(char **args, t_env **env);

void	print_sorted_env(t_env *env);

int		env_size(t_env *env);
t_env	*copy_env_list(t_env *env);
t_env	*search_env_key(t_env *env, const char *key);

/* Utils */
int		count_args(char **args);

#endif