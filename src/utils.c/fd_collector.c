/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_collector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:26:55 by arocca            #+#    #+#             */
/*   Updated: 2025/04/13 09:55:47 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	open_fd(t_fd **head, int fd)
{
	t_fd *new;

	new = malloc(sizeof(t_fd));
	if (!new)
		return ;
	new->fd = fd;
	new->next = *head;
	*head = new;
}

void	close_fd(t_fd **head, int fd)
{
	t_fd *tmp;
	t_fd **curr;

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

void	close_fd(t_fd **head, int fd)
{
	t_fd 	*tmp;

	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		close(tmp->fd);
		free(tmp);
	}
}
