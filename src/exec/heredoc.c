/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:03:44 by arocca            #+#    #+#             */
/*   Updated: 2025/04/29 14:54:04 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

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
			break ; // EOF (Ctrl-D)
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

int	prepare_redirections(t_ctx *ctx, t_ast *node)
{
	int	fd;

	if (node == NULL)
		return (1);
	if (node->type == AST_PIPE)
	{
		if (!prepare_redirections(ctx, node->childs[0]))
			return (0);
		if (!prepare_redirections(ctx, node->childs[1]))
			return (0);
	}
	else if (node->type == AST_REDIR)
	{
		if (ft_strcmp(node->value, "<<") == 0)
		{
			fd = here_doc(node->childs[0]->value);
			if (fd < 0)
				return (perr("heredoc", 1), 0);
			register_fd(&ctx->fds, fd);
		}
		else if (ft_strcmp(node->value, "<") == 0)
		{
			fd = open_fd(&ctx->fds, node->childs[0]->value, O_RDONLY, 0);
			if (fd < 0)
				return (ft_dprintf(2, "minishell : "), perr(node->childs[0]->value, 1));
		}
		else if (ft_strcmp(node->value, ">") == 0)
		{
			fd = open_fd(&ctx->fds, node->childs[0]->value, TRUNC_FLAGS, 0644);
			if (fd < 0)
				return (ft_dprintf(2, "minishell : "), perr(node->childs[0]->value, 1));
		}
		else if (ft_strcmp(node->value, ">>") == 0)
		{
			fd = open_fd(&ctx->fds, node->childs[0]->value, APPEND_FLAGS, 0644);
			if (fd < 0)
				return (ft_dprintf(2, "minishell : "), perr(node->childs[0]->value, 1));
		}
		if (!prepare_redirections(ctx, node->childs[1]))
			return (0);
	}
	return (1);
}
