/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:58:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/22 16:00:19 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* -- Includes -- */

/* -- Structures -- */
typedef struct	s_ctx
{
    char    **env;		// pointeur vers ton tableau d’env
    int      stdin_fd;	// pour restaurer après redir
    int      stdout_fd;	// idem
}			t_ctx;

/* -- Functions -- */

#endif