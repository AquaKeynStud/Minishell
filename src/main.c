/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/18 12:19:19 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"
#include "sigaction.h"

int	main(void)
{
	set_sigaction(SIGQUIT, handle_sigint_sigquit, "\0");
	set_sigaction(SIGINT, handle_sigint_sigquit, "1000000");
	get_input_loop();
	return (0);
}
