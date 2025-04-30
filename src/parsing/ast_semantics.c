/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_semantics.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:37:20 by arocca            #+#    #+#             */
/*   Updated: 2025/04/30 09:44:22 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

void	*parsing_err(t_ctx *ctx, const char *msg, int code)
{
	if (code >= 0)
		ctx->status = code;
	ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n", msg);
	return (NULL);
}

/*
** redir_priority : Assure la hiérarchie des redirections.
** @cmd: Adresse du pointeur sur l'ast courant.
** @redir: Adresse du noeud ast de la nouvelle redirection.
** Imbrique la nouvelle redirection après les précédentes,
** ou imbrique la commande dans la redirection si c'est la
** première 
*/
void	redir_priority(t_ast **cmd, t_ast *redir)
{
	t_ast	*leaf;
	t_ast	*parent;

	if (*cmd && (*cmd)->type == AST_REDIR)
	{
		parent = *cmd;
		leaf = parent->childs[1];
		while (leaf && leaf->type == AST_REDIR)
		{
			parent = leaf;
			leaf = leaf->childs[1];
		}
		if (!leaf)
			return ;
		ast_add_child(redir, leaf);
		parent->childs[1] = redir;
	}
	else
	{
		ast_add_child(redir, *cmd);
		*cmd = redir;
	}
}

/*
** overwrite_stub : Overwrite le potentiel stub des redirections orphelines.
** @curr: Adresse du pointeur sur le token courant.
** @stub: Adresse du stub (Noeud vide nécessaire pour créer les redirections).
** @cmd: La commande qui va remplacer le stub
** Remplace le contenu vide dans le stub pour le transformer en commande.
*/
t_ast	*overwrite_stub(t_token **curr, t_ast **cmd)
{
	t_ast	*stub;

	if (!*cmd)
		*cmd = new_ast(AST_COMMAND, (*curr)->value);
	else
	{
		stub = *cmd;
		while (stub->type == AST_REDIR)
			stub = stub->childs[1];
		stub->value = ft_strdup((*curr)->value);
	}
	stub = *cmd;
	while (stub->type == AST_REDIR)
		stub = stub->childs[1];
	*curr = (*curr)->next;
	return (stub);
}

void	cat_empty_heredoc(t_ast **cmd, t_token *tmp)
{
	t_ast	*stub;

	if (!*cmd)
	{
		if (tmp->type == TOKEN_HEREDOC)
			*cmd = new_ast(AST_COMMAND, "cat");
		else
			*cmd = new_ast(AST_COMMAND, NULL);
		return ;
	}
	if (tmp->type == TOKEN_HEREDOC)
	{
		stub = *cmd;
		while (stub->type == AST_REDIR)
			stub = stub->childs[1];
		if (!stub->value)
			stub->value = ft_strdup("cat");
	}
}
