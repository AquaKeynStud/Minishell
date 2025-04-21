/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_setters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:49:18 by arocca            #+#    #+#             */
/*   Updated: 2025/04/21 11:19:32 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "lexing.h"
#include "parsing.h"

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
** parse_redirs : Parse les redirections.
** @cmd: Adresse vers le noeud créé qui contient la commande.
** @curr: Adresse du pointeur sur le token courant.
** Retourne un nœud AST_COMMAND ou NULL en cas d'erreur.
*/
void	parse_redirs(t_ast	**cmd, t_token **curr)
{
	t_token	*tmp;
	t_ast	*redir;

	while (*curr && ((*curr)->type == TOKEN_REDIR_IN || (*curr)->type == TOKEN_REDIR_OUT)) // Gestion des redirections : > ou <
	{
		tmp = *curr;
		*curr = (*curr)->next;
		if (*curr && (*curr)->type == TOKEN_WORD)
		{
			redir = new_ast(AST_REDIR, tmp->value); // Crée un nœud redirection et l'attache comme enfant de la commande
			ast_add_child(redir, new_ast(AST_COMMAND, (*curr)->value));
			ast_add_child((*cmd), redir);
			*curr = (*curr)->next;
		}
		else
		{
			err("minishell: Syntax error near unexpected token `newline'\n");
			// besoin de free ici
			return ;
		}
	}
}

/*
** parse_command : Parse une commande simple.
** @curr: Adresse du pointeur sur le token courant.
** Retourne un nœud AST_COMMAND ou NULL en cas d'erreur.
*/
t_ast	*parse_command(t_token **curr)
{
	t_ast	*cmd;

	if (!curr || !(*curr) || (*curr)->type != TOKEN_WORD)
		return (NULL);
	cmd = new_ast(AST_COMMAND, (*curr)->value); // Crée le nœud de la commande (premier mot = nom de commande)
	*curr = (*curr)->next;
	while (*curr && (*curr)->type == TOKEN_WORD) // Tant que c'est un argument
	{
		ast_add_child(cmd, new_ast(AST_COMMAND, (*curr)->value));
		*curr = (*curr)->next;
	}
	parse_redirs(&cmd, curr);
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
t_ast	*parse_pipeline(t_token **curr)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(curr);
	if (!left)
		return (NULL);
	while (*curr && (*curr)->type == TOKEN_PIPE)
	{
		*curr = (*curr)->next; // Consomme le token pipe
		right = parse_command(curr);
		if (!right)
		{
			err("Error : Syntax error near unexpected token `pipe'\n");
			return (NULL);
		}
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
t_ast	*parse_input(t_token *tokens)
{
	t_ast	*ast;
	t_token	*curr;

	curr = tokens;
	ast = parse_pipeline(&curr);
	if (curr != NULL)
	{
		err("Erreur: tokens non consommés en fin de parsing\n");
		return (NULL);
	}
	return (ast);
}
