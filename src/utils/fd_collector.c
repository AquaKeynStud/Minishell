/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_collector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/04/25 14:02:28 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 *	FD_COLLECTOR
 *
 * Paramètres :
 * 		- Head 		: Pointeur vers la liste chaînée des fds stockés
 * 		- filepath	: Chemin vers le fichier à ouvrir
 * 		- flags		: flags parmi les suivants :
 * 						obligatoire (1 seul):
 * 							- O_RDONLY	(lecture seule) = 0
 * 							- O_WRONLY	(écriture seule) = 1
 * 							- O_RDWR	(lecture + écriture) = 2
 * 						optionnels (avec |):
 * 							O_CREAT (64), O_EXCL (128), O_NOCTTY (256), O_TRUNC (512), O_APPEND (1024),
 * 							O_NONBLOCK (2048), O_DSYNC (4096), FASYNC (8192), O_DIRECT (16384), O_LARGEFILE (32768),
 * 							O_DIRECTORY (65536), O_NOFOLLOW (131072), O_CLOEXEC (524288), O_SYNC (1052672),
 * 							O_PATH (2097152), O_TMPFILE (4259840)
 * 		- perms		: Numéro de permissions (0000~0777)
 */

#include "minishell.h"

int	open_fd(t_fd **head, const char *filepath, int flags, mode_t perms)
{
	t_fd	*new;
	int		fd;

	fd = open(filepath, flags, perms);
	if (fd < 0)
		return (-1);
	new = malloc(sizeof(t_fd));
	if (!new)
		return (close(fd) - 1);
	new->fd = fd;
	new->next = *head;
	*head = new;
	return (fd);
}

int	register_fd(t_fd **head, int fd)
{
	t_fd	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (1);
	new->fd = fd;
	new->next = *head;
	*head = new;
	return (0);
}

void	close_fd(t_fd **head, int fd)
{
	t_fd	*tmp;
	t_fd	**curr;

	curr = head;
	while (*curr)
	{
		if ((*curr)->fd == fd)
		{
			tmp = *curr;
			*curr = (*curr)->next;
			close(tmp->fd);
			free(tmp);
			return ;
		}
		curr = &((*curr)->next);
	}
}

void	close_all_fds(t_fd **head)
{
	t_fd	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		close(tmp->fd);
		free(tmp);
		tmp = NULL;
	}
}
