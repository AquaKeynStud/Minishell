/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:49:18 by arocca            #+#    #+#             */
/*   Updated: 2025/05/01 10:35:14 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

/*
** ============================================================================
** 3. Fonctions de Parsing
**
** Ces fonctions transforment la liste chainée des tokens en un AST.
**
** - parse_command:
**      Lit une commande simple (TOKEN_WORD) puis ses arguments et redirections.
**      Schéma:
**           [TOKEN_WORD] => AST_COMMAND ("commande")
**               └── [arguments...]
**               └── [AST_REDIRECTION (>)]
**
** - parse_pipeline:
**      Gère un pipeline en appelant parse_command et en enveloppant
**      les commandes avec un nœud AST_PIPE.
**      Schéma:
**             AST_PIPE ("|")
**              /          \
**    AST_COMMAND      AST_COMMAND
**
** - parse_input:
**      Fonction d'entrée qui démarre le parsing et vérifie que tous
**      les tokens ont été consommés.
** ============================================================================
*/

/*
** parse_redirs : traite une redirection unique
** @ctx   : contexte de parsing
** @cmd   : adresse du pointeur vers l'AST de la commande
** @curr  : pointeur sur le token courant (redir op)
** Retourne 1 si OK, 0 sinon (et libère AST en erreur).
*/
/*
** parse_redirs : traite une redirection unique
** @ctx   : contexte de parsing
** @cmd   : adresse du pointeur vers l'AST de la commande
** @curr  : pointeur sur le token courant (redir op)
** Retourne 1 si OK, 0 sinon (et libère AST en erreur).
*/
static int	parse_redirs(t_ctx *ctx, t_ast **cmd, t_token **curr)
{
	t_token	*tmp;
	t_ast	*file_node;
	t_ast	*redir;

	if (!*curr)
		return (1);
	tmp = *curr;
	*curr = (*curr)->next;
	if (*curr && ((*curr)->type != TOKEN_WORD))
	{
		if ((*curr)->type != TOKEN_PIPE)
			return (parsing_err(ctx, (*curr)->value, 2));
		return (0);
	}
	if (!*curr || ((*curr)->type != TOKEN_WORD))
		return (parsing_err(ctx, "newline", 2));
	cat_empty_heredoc(cmd, tmp);
	file_node = new_ast(AST_COMMAND, (*curr)->value);
	redir = new_ast(AST_REDIR, tmp->value);
	ast_add_child(redir, file_node);
	redir_priority(cmd, redir);
	*curr = (*curr)->next;
	return (1);
}

/*
** parse_command : Parse une commande simple avec redirections et arguments
** Parcours en une seule passe jusqu'à TOKEN_PIPE ou fin.
*/
static t_ast	*parse_command(t_ctx *ctx, t_token **curr)
{
	t_ast	*cmd;
	t_ast	*stub;

	cmd = NULL;
	while (*curr && (*curr)->type != TOKEN_PIPE)
	{
		if ((*curr)->type != TOKEN_WORD && (*curr)->type != TOKEN_PIPE)
		{
			if (!parse_redirs(ctx, &cmd, curr))
				return (NULL);
		}
		else if ((*curr)->type == TOKEN_WORD)
		{
			if (!cmd)
				stub = overwrite_stub(curr, &cmd);
			else
			{
				ast_add_child(stub, new_ast(AST_COMMAND, (*curr)->value));
				*curr = (*curr)->next;
			}
		}
		else
			break ;
	}
	return (cmd);
}

/*
** parse_pipeline : Parse un ensemble de commandes séparées par des pipes.
** @curr: Adresse du pointeur sur le token courant.
** Retourne un AST représentant le pipeline.
** Schéma:
**             AST_PIPE ("|")
**             /            \
**   parse_command()   parse_command()
*/
static t_ast	*parse_pipeline(t_ctx *ctx, t_token **curr)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(ctx, curr);
	if (!left)
		return (NULL); // Erreur déjà affichée
	if (!left->value)
		return (free_ast(left));
	while (*curr && (*curr)->type == TOKEN_PIPE)
	{
		*curr = (*curr)->next; // Consomme le token pipe
		right = parse_command(ctx, curr);
		if (!right || !right->value)
			return (double_free_ast(right, left));
		pipe_node = new_ast(AST_PIPE, "|"); // Crée un nœud pipe rassemblant left et right
		ast_add_child(pipe_node, left);
		ast_add_child(pipe_node, right);
		left = pipe_node; // Le nouveau noeud pipe devient le nœud de gauche pour un pipeline plus long
	}
	return (left);
}

/*
** parse_input : Point d'entrée du parsing.
** @tokens: Pointeur sur la liste chainée des tokens.
** Retourne l'AST complet ou NULL en cas d'erreur.
** Vérifie que tous les tokens ont été consommés.
*/
t_ast	*parse_input(t_ctx *ctx, t_token *tokens)
{
	t_ast	*ast;
	t_token	*curr;

	curr = tokens;
	ast = parse_pipeline(ctx, &curr);
	if (curr != NULL)
	{
		parsing_err(ctx, "|", 2);
		return (NULL);
	}
	return (ast);
}
