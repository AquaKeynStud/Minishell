/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/28 09:12:21 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	READLINE_H
# define READLINE_H

#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

typedef struct	s_env
{
	char *key;			// ex: "PATH"
	char *value;		// ex: "/usr/bin:/bin"
	struct s_env *next;
} t_env;

/* Prototypes */
int		ft_echo(char **args);

void	malloc_fail(void);
void	free_split(char **split);

/* ft_export */
int	ft_export(char **args, t_env **env);
void	sort(t_env **env);

int		env_size(t_env *env);
t_env	*copy_env_list(t_env *env);
void	swap_env_value(t_env *a, t_env *b);
void	print_sorted_env(t_env *env);

/* Utils */
int count_args(char **args);

#endif