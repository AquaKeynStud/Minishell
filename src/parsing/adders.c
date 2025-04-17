/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:01:03 by arocca            #+#    #+#             */
/*   Updated: 2025/04/09 18:13:06 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	add_command(t_command **head, t_command *new_cmd)
{
	if (!head || !new_cmd)
		return;
	if (!*head)
		*head = new_cmd;
	else
	{
		t_command *current = *head;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
}

void	add_redirection(t_command *cmd, t_redir type, const char *file)
{
	t_redirection	*current;
	t_redirection	*new_redir;

	new_redir = create_redirection(type, file);
	if (!new_redir)
		return ;
	if (!cmd->redirs)
		cmd->redirs = new_redir;
	else
	{
		current = cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}

char	**add_argument(char **args, const char *arg)
{
	int	count;
	
	count = 0;														// Compter le nombre d'arguments actuels
	while (args && args[count])
		count++;
	char **new_args = ft_realloc(args, sizeof(char *) * (count + 2));	// +2 car on garde la place pour le NULL de fin
	if (!new_args)
		return (args);  											// En cas d'erreur, on retourne l'ancien tableau
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;										// NULL terminated
	return (new_args);
}
