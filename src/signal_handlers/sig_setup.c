/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:06:29 by arocca            #+#    #+#             */
/*   Updated: 2025/04/07 10:22:10 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // Pour perror
#include <stddef.h> // Pour le NULL, à mettre dans le header si réutilisé
#include "sigaction.h"

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

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask); // Masque vide
	get_sa_flags(&sa, flags); // Prend les flags avec une série de 7 fois 0 ou 1 qui correspondent chacun à un flag (ex : "0110010")
	if (sigaction(signum, &sa, NULL))
		return (true);
	return (false);
}
