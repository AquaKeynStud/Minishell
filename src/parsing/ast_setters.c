/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_setters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:49:18 by arocca            #+#    #+#             */
/*   Updated: 2025/04/18 14:01:08 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
** parse_command : Parse une commande simple.
** @current: Adresse du pointeur sur le token courant.
** Retourne un nœud AST_COMMAND ou NULL en cas d'erreur.
*/
t_ast	*parse_command(t_token **current)
{
	t_ast	*cmd;

	if (!current || !(*current) || (*current)->type != TOKEN_WORD)
		return (NULL);
	/* Crée le nœud de la commande (premier mot = nom de commande) */
	cmd = new_ast(AST_COMMAND, (*current)->value);
	*current = (*current)->next;
	/* Boucle sur les tokens de type TOKEN_WORD pour traiter les arguments */
	while (*current && (*current)->type == TOKEN_WORD)
	{
		ast_add_child(cmd, new_ast(AST_COMMAND, (*current)->value));
		*current = (*current)->next;
	}
	/* Gestion des redirections : > ou < */
	while (*current && ((*current)->type == TOKEN_REDIR_IN ||
				(*current)->type == TOKEN_REDIR_OUT))
	{
		t_token	*tmp;

		tmp = *current;
		*current = (*current)->next;
		if (*current && (*current)->type == TOKEN_WORD)
		{
			/* Crée un nœud redirection et l'attache comme enfant de la commande */
			t_ast	*redir = new_ast(AST_REDIR, tmp->value);
			ast_add_child(redir, new_ast(AST_COMMAND, (*current)->value));
			ast_add_child(cmd, redir);
			*current = (*current)->next;
		}
		else
		{
			fprintf(stderr, "Erreur de syntaxe: redirection sans cible\n");
			return (NULL);
		}
	}
	return (cmd);
}

/*
** parse_pipeline : Parse un ensemble de commandes séparées par des pipes.
** @current: Adresse du pointeur sur le token courant.
** Retourne un AST représentant le pipeline.
** Schéma:
**             AST_PIPE ("|")
**             /            \
**   parse_command()   parse_command()
*/
t_ast	*parse_pipeline(t_token **current)
{
	t_ast	*left;
	t_ast	*pipe_node;
	t_ast	*right;

	left = parse_command(current);
	if (!left)
		return (NULL);
	while (*current && (*current)->type == TOKEN_PIPE)
	{
		/* Consomme le token pipe */
		*current = (*current)->next;
		right = parse_command(current);
		if (!right)
		{
			fprintf(stderr, "Erreur de syntaxe après le pipe\n");
			return (NULL);
		}
		/* Crée un nœud pipe rassemblant left et right */
		pipe_node = new_ast(AST_PIPE, "|");
		ast_add_child(pipe_node, left);
		ast_add_child(pipe_node, right);
		/* Le nouveau noeud pipe devient le nœud de gauche pour un pipeline plus long */
		left = pipe_node;
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
	t_token	*current;

	current = tokens;
	t_ast	*ast = parse_pipeline(&current);
	if (current != NULL)
	{
		fprintf(stderr, "Erreur: tokens non consommés en fin de parsing\n");
		return (NULL);
	}
	return (ast);
}
