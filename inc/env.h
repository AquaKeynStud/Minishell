/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:13:44 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/20 15:11:11 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	ENV_H
# define ENV_H

/* -- Includes -- */
#include "libft.h"
#include <stdbool.h>

/* -- Structures -- */
typedef struct	s_env
{
	char *key;			// ex: "PATH"
	char *value;		// ex: "/usr/bin:/bin"
	struct s_env *next;
}				t_env;

/* -- Functions -- */
int		ft_echo(char **args);

void	free_split(char **split);

#endif