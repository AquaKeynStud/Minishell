/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_collector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/04/29 23:01:47 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
