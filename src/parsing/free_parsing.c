/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 11:40:35 by arocca            #+#    #+#             */
/*   Updated: 2025/04/09 11:40:43 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void free_redirections(t_redirection *redir) {
    t_redirection *tmp;
    while (redir) {
        tmp = redir->next;
        if (redir->file)
            free(redir->file);
        free(redir);
        redir = tmp;
    }
}

void free_commands(t_command *cmd) {
    t_command *tmp;
    int i;
    while (cmd) {
        tmp = cmd->next;
        if (cmd->name)
            free(cmd->name);
        if (cmd->args) {
            for (i = 0; cmd->args[i] != NULL; i++) {
                free(cmd->args[i]);
            }
            free(cmd->args);
        }
        free_redirections(cmd->redirs);
        free(cmd);
        cmd = tmp;
    }
}
