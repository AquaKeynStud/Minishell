/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/03 14:47:22 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "readline.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell > ");
		if (!input)
			break ;
		if (!*input == '\0')
			add_history(input);
		free(input);
	}
	rl_clear_history();
	return (0);
}
