/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:46:36 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/15 09:26:17 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <sys/wait.h>

static char	**get_files_from_pipe(t_ctx *ctx, int fd)
{
	int		i;
	char	*line;
	char	**res;

	res = s_malloc(ctx, sizeof(char *) * 1024);
	if (!res)
		return (NULL);
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (line[0])
		{
			ft_trim(&line, "\n");
			res[i++] = line;
		}
		else
			free(line);
		line = get_next_line(fd);
	}
	res[i] = NULL;
	return (res);
}

static void	init_get_files(char ***res, char **argv, char **envp)
{
	*res = NULL;
	argv[0] = "/bin/ls";
	argv[1] = "-1";
	argv[2] = NULL;
	envp[0] = NULL;
}

char	**get_files(t_ctx *ctx)
{
	pid_t	pid;
	char	**res;
	char	*argv[3];
	char	*envp[1];
	int		pipefd[2];

	init_get_files(&res, argv, envp);
	if (pipe(pipefd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execve(argv[0], argv, envp);
		secure_exit(ctx);
	}
	close(pipefd[1]);
	res = get_files_from_pipe(ctx, pipefd[0]);
	close_unregistered_fds(ctx);
	wait(NULL);
	return (res);
}
