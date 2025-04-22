/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 11:11:20 by abouclie         ###   ########.fr       */
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
int ft_echo(char **args);

void	malloc_fail(void);
void	free_split(char **split);

#endif