/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigaction.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:11:58 by arocca            #+#    #+#             */
/*   Updated: 2025/04/04 18:14:50 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGACTION_H
# define SIGACTION_H

/* -- Includes -- */
#include <signal.h>

/* -- Setup functions -- */
void	setup_sigaction(int signum, void (*handler)(int), char *flags);

/* -- Handlers functions -- */
void	handle_sigint_sigquit(int signal);

#endif