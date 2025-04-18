/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/18 12:46:40 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	READLINE_H
# define READLINE_H

#include <stdlib.h>

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