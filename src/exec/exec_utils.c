/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:04:52 by arocca            #+#    #+#             */
/*   Updated: 2025/04/28 13:32:29 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parsing.h"
#include "minishell.h"
#include "sigaction.h"
#include <readline/history.h>
#include <readline/readline.h>

char	**ast_to_argv(t_ast *node)
{
	int		pos;
	char	**argv;
	t_ast	**childs;

	pos = node->sub_count;
	childs = node->childs;
	argv = s_malloc((pos + 2) * sizeof(char *));
	argv[pos + 1] = NULL;
	argv[0] = node->value;
	while (pos > 0)
	{
		argv[pos] = childs[pos - 1]->value;
		pos--;
	}
	return (argv);
}

char	*get_path(const char *cmd, t_env *env)
{
	int		i;
	char	**paths;
	char	*raw_path;
	char	*full_cmd;

	i = 0;
	raw_path = get_from_env(env, "PATH");
	if (ft_strchr(cmd, '/') || !raw_path)
		return (ft_strdup(cmd));
	paths = ft_split(raw_path, ':');
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_cmd = join_with_delim(paths[i++], cmd, "/");
		if (!full_cmd || !access(full_cmd, X_OK))
			break ;
		free(full_cmd);
		full_cmd = NULL;
	}
	double_free((void **)paths, 0);
	return (full_cmd);
}

int	free_cmd(char *path, char **args, char **envp, int exit_code)
{
	if (path)
		free(path);
	if (args)
		free(args);
	if (envp)
		double_free((void **)envp, 0);
	return (exit_code);
}

/**
 * here_doc: lit un here-document via readline, sans expansion
 * @limiter: chaîne de fin du heredoc
 *
 * Retourne la borne de lecture du pipe (à dupliquer sur STDIN),
 * ou -1 en cas d'erreur.
 */
int	here_doc(const char *limiter)
{
	char	*line;
	char	*prompt;
	int	 	pipefd[2];

	if (pipe(pipefd) < 0)
		return (-1);
	prompt = NULL;
	if (isatty(STDIN_FILENO)) // Prompt seulement si on est en interractif
		prompt = "> ";
	while (1)
	{
		line = readline(prompt);
		if (!line)
			break; // EOF (Ctrl-D)
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
