/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_sigaction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:06:29 by arocca            #+#    #+#             */
/*   Updated: 2025/04/03 15:39:33 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

bool	parse_flags(char *s)
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

void	get_sa_flags(struct sigaction *sa, char *flags)
{
	if (!parse_flags(flags))
		return ;
	if (flags[0] == '1')
		sa.sa_flags |= SA_RESTART;
	if (flags[1] == '1')
		sa.sa_flags |= SA_NOCLDSTOP;
	if (flags[2] == '1')
		sa.sa_flags |= SA_NOCLDWAIT;
	if (flags[3] == '1')
		sa.sa_flags |= SA_SIGINFO;
	if (flags[4] == '1')
		sa.sa_flags |= SA_NODEFER;
	if (flags[5] == '1')
		sa.sa_flags |= SA_ONSTACK;
	if (flags[6] == '1')
		sa.sa_flags |= SA_RESETHAND;
}

void	setup_sigaction(int signum, void (*handler)(int), char *flags)
{
	struct sigaction	*sa;
	
	if (ft_strlen)
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask); // Masque vide
	get_sa_flags(sa, flags); // Prend les flags avec une chaine de 7 "0000000" qui correspondent chacun à un flag

	if (sigaction(signum, &sa, NULL))
		return (perror("sigaction"));
}