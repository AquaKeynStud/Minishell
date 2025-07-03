/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigaction.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:11:58 by arocca            #+#    #+#             */
/*   Updated: 2025/05/21 10:55:08 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGACTION_H
# define SIGACTION_H

/* -- Includes -- */
# include <signal.h>
# include <stdbool.h>

/* -- Setup functions -- */
void	sig_init(void);
void	sig_set(void (*handler)(int));
bool	set_sigaction(int signum, void (*handler)(int), char *flags);

/* -- Handlers functions -- */
void	handle_sigint(int signal);
void	handle_sigint_heredoc(int signal);

#endif