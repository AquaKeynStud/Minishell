/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creators.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 11:39:11 by arocca            #+#    #+#             */
/*   Updated: 2025/04/09 18:17:07 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "parsing.h"

t_command	*create_command(const char *name)
{
	t_command	*cmd;
	
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->name = ft_strdup(name);	// copie du nom de la commande
	cmd->args = NULL;				// tableau d'arguments initial vide
	cmd->redirs = NULL;				// pas de redirections au départ
	cmd->next = NULL;
	return (cmd);
}

t_redirection	*create_redirection(t_redir type, const char *file)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;						// Type de redirection (>/</>>/<<)
	redir->file = ft_strdup(file);			// Fichier vers lequel on redirige
	redir->next = NULL;
	return (redir);
}
