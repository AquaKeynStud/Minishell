/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/05 19:21:36 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "readline.h"
#include "sigaction.h"

int	main(void)
{
	char	*input;

	set_sigaction(SIGQUIT, handle_sigint_sigquit, NULL);
	// set_sigaction(SIGINT, handle_sigint_sigquit, "1000000");
	while (1)
	{
		input = readline("\033[1m\033[32mminishell \033[31m➤ \033[0m");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		free(input);
	}
	rl_clear_history();
	return (0);
}
