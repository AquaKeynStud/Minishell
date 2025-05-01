/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:06:29 by arocca            #+#    #+#             */
/*   Updated: 2025/05/01 22:40:07 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // Pour perror
#include <stddef.h>
#include "sigaction.h"

/*
 * Gives a status to all the signals :
 *				ex : SIG_IGN, SIG_DFL to restore them to default...
 * The status is the handler.
 */
void	sig_set(void (*handler)(int))
{
	set_sigaction(SIGINT, handler, NULL); // Désactive mes fonctions sigaction de signal
	set_sigaction(SIGQUIT, handler, NULL);
}

void	sig_init(void)
{
	set_sigaction(SIGQUIT, SIG_IGN, NULL);
	set_sigaction(SIGINT, handle_sigint_sigquit, "1000000");
}

static bool	parse_flags(char *s)
{
	int	i;

	i = 0;
	while (*s)
	{
		if ((*s != '0' && *s != '1') || i > 7)
			return (false);
		s++;
		i++;
	}
	if (i != 7)
		return (false);
	return (true);
}

static void	get_sa_flags(struct sigaction *sa, char *flags)
{
	sa->sa_flags = 0;
	if (!*flags || !parse_flags(flags))
		return ;
	if (flags[0] == '1')
		sa->sa_flags |= SA_RESTART;
	if (flags[1] == '1')
		sa->sa_flags |= SA_NOCLDSTOP;
	if (flags[2] == '1')
		sa->sa_flags |= SA_NOCLDWAIT;
	if (flags[3] == '1')
		sa->sa_flags |= SA_SIGINFO;
	if (flags[4] == '1')
		sa->sa_flags |= SA_NODEFER;
	if (flags[5] == '1')
		sa->sa_flags |= SA_ONSTACK;
	if (flags[6] == '1')
		sa->sa_flags |= SA_RESETHAND;
}

bool	set_sigaction(int signum, void (*handler)(int), char *flags)
{
	struct sigaction	sa;

	if (!flags)
		flags = "\0";
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask); // Masque vide
	get_sa_flags(&sa, flags); // Prend les flags avec une série de 7 fois 0 ou 1 qui correspondent chacun à un flag (ex : "0110010")
	if (sigaction(signum, &sa, NULL))
		return (true);
	return (false);
}
