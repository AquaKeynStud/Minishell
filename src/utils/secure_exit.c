/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:15:31 by arocca            #+#    #+#             */
/*   Updated: 2025/04/26 21:01:48 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

// Fonction qui va permettre de sortir n'importe quand
void	secure_exit(t_ctx *ctx, unsigned char code)
{
	free_env(&ctx->env);
	close_all_fds(&ctx->fds); // close de tous les fds ouverts
	close(ctx->stdin_fd);
	close(ctx->stdout_fd);
	exit(code);
}

int	s_exec_exit(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}
