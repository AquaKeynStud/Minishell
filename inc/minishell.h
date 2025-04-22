/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:58:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/22 16:52:06 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* -- Includes -- */
#include "utils.h"
#include "env.h"

/* -- Structures -- */
typedef struct	s_ctx
{
	t_env	*env;		// pointeur vers ton tableau d’env
	t_fd	*fds;
	int		stdin_fd;
	int		stdout_fd;
	int		status;
}			t_ctx;

/* -- Functions -- */

#endif