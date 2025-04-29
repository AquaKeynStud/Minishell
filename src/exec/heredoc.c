/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:03:44 by arocca            #+#    #+#             */
/*   Updated: 2025/04/29 19:46:03 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	verif_redir(t_ctx *ctx, t_ast *node)
{
	int	fd;

	if (!ft_strcmp(node->value, "<"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, O_RDONLY, 0);
	else if (!ft_strcmp(node->value, ">"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, TRUNC_FLAGS, 0644);
	else if (!ft_strcmp(node->value, ">>"))
		fd = open_fd(&ctx->fds, node->childs[0]->value, APPEND_FLAGS, 0644);
	if (fd < 0)
	{
		ft_dprintf(2, "minishell : ");
		perr(node->childs[0]->value, 1);
		return ;
	}
	close_fd(&ctx->fds, fd);
}

/**
 * here_doc: lit un here-document via readline, sans expansion
 * @limiter: chaîne de fin du heredoc
 *
 * Retourne la borne de lecture du pipe (à dupliquer sur STDIN),
 * ou -1 en cas d'erreur.
 */
static int	here_doc(const char *limiter)
{
	char	*line;
	char	*prompt;
	int		pipefd[2];

	if (pipe(pipefd) < 0)
		return (-1);
	prompt = NULL;
	if (isatty(STDIN_FILENO)) // Prompt seulement si on est en interractif
		prompt = "> ";
	while (1)
	{
		line = readline(prompt);
		if (!line)
			break ;
		if (!ft_strcmp(line, limiter)) // Fin de heredoc
		{
			free(line);
			break ;
		}
		ft_dprintf(pipefd[1], "%s\n", line); // Écrire la ligne dans le pipe, en restaurant le '\n'
		free(line);
	}
	close(pipefd[1]); // On termine l'écriture et on renvoie le descripteur de lecture
	return (pipefd[0]);
}

int	get_redir(t_ctx *ctx, t_ast *ast)
{
	int	fd;

	if (!ast)
		return (1);
	if (ast->type == AST_PIPE)
	{
		if (!get_redir(ctx, ast->childs[0]) || !get_redir(ctx, ast->childs[1]))
			return (0);
	}
	else if (ast->type == AST_REDIR)
	{
		if (ft_strcmp(ast->value, "<<") == 0)
		{
			fd = here_doc(ast->childs[0]->value);
			if (fd < 0)
				return (perr("heredoc", 1), 0);
			ast->fd = fd;
			register_fd(&ctx->fds, fd);
		}
		else
			verif_redir(ctx, ast);
		if (!get_redir(ctx, ast->childs[1]))
			return (0);
	}
	return (1);
}
