/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:56:52 by arocca            #+#    #+#             */
/*   Updated: 2025/05/02 00:23:14 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include "minishell.h"

static int	is_dir(const char *path)
{
	struct stat	sb;

	return (!stat(path, &sb) && S_ISDIR(sb.st_mode));
}

static void	enoent_err(t_ctx *ctx, const char *value)
{
	ctx->status = 127;
	if (value && ft_strchr(value, '/'))
	{
		ft_dprintf(2, "minishell: ");
		perror(value);
	}
	else if (value)
		ft_dprintf(2, "minishell: %s: command not found\n", value);
	else
		ft_dprintf(2, "minishell: command not found\n");
}

static void	eacces_err(t_ctx *ctx, const char *value)
{
	ctx->status = 126;
	if (value && is_dir(value))
		ft_dprintf(2, "minishell: %s: Is a directory\n", value);
	else if (value)
	{
		ft_dprintf(2, "minishell: ");
		perror(value);
	}
	else
		perror("minishell");
}

int	execve_err(t_ctx *ctx, char **value)
{
	int	saved_errno;

	ctx->status = 1;
	saved_errno = errno;
	if (saved_errno == ENOENT)
		enoent_err(ctx, *value);
	else if (saved_errno == EACCES)
		eacces_err(ctx, *value);
	else
	{
		if (value && *value)
			perror(*value);
		else
			perror("minishell");
	}
	return (ctx->status);
}

int	redir_err(t_ctx *ctx, t_ast *ast, int exit_code)
{
	ft_dprintf(2, "minishell: ");
	perror(ast->childs[0]->value);
	ctx->status = 1;
	if (exit_code < 0)
		return (exit_code);
	return (1);
}
