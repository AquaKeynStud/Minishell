/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files_wildcards.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:46:36 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/14 12:15:09 by abouclie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <sys/wait.h>

static char	**get_files_from_pipe(int fd)
{
	char	**res;
	char	*line;
	int		i;

	res = malloc(sizeof(char *) * 1024);
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

char	**get_files()
{
	int		pipefd[2];
	pid_t	pid;
	char	*argv[3];
	char	*envp[1];
	char	**res;

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
		exit(1);
	}
	close(pipefd[1]);
	res = get_files_from_pipe(pipefd[0]);
	close(pipefd[0]);
	wait(NULL);
	return (res);
}
