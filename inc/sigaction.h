/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigaction.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:11:58 by arocca            #+#    #+#             */
/*   Updated: 2025/04/05 18:21:47 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGACTION_H
# define SIGACTION_H

/* -- Includes -- */
#include <signal.h>
#include <stdbool.h>

/* -- Setup functions -- */
bool	set_sigaction(int signum, void (*handler)(int), char *flags);

/* -- Handlers functions -- */
void	handle_sigint_sigquit(int signal);

#endif