/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_adders.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:07:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/22 17:09:53 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

/*
** ============================================================================
** 2. Fonctions d'allocation et d'ajout de nœud dans l'AST
**
** La fonction new_ast crée un nœud en dupliquant la valeur fournie.
** La fonction ast_add_child ajoute dynamiquement un enfant à un nœud.
**
** Schéma de new_ast:
**    input: type et valeur
**    output: pointeur sur le nœud nouvellement alloué.
**
** Schéma de ast_add_child:
**    parent --------------> enfant à ajouter
**    (le tableau des enfants est réalloué)
** ============================================================================
*/

/*
** new_ast : Alloue un nouveau nœud AST.
** @type: type de nœud (AST_COMMAND, AST_PIPE, AST_REDIRECTION)
** @value: chaîne associée (commande, redirection, etc.)
** Retourne un pointeur sur le nœud ou quitte en cas d'erreur.
*/
t_ast	*new_ast(t_ctx ctx, t_ast_type type, const char *value)
{
	t_ast	*node;

	node = (t_ast *)s_malloc(sizeof(t_ast));
	node->type = type;
	node->value = NULL;
	if (value)
		node->value = ft_strdup(value);
	node->sub_count = 0;
	node->childs = NULL;
	return (node);
}

/*
** ast_add_child : Ajoute un enfant au nœud AST parent.
** @parent: Le nœud parent auquel ajouter l'enfant.
** @child: Le nœud enfant à ajouter.
** Réalloue le tableau des enfants et incrémente sub_count.
*/
void	ast_add_child(t_ctx ctx, t_ast *parent, t_ast *child)
{
	size_t	old_size;
	size_t	new_size;

	old_size = sizeof(t_ast *) * parent->sub_count;
	new_size = sizeof(t_ast *) * (++parent->sub_count);
	parent->childs = s_realloc(parent->childs, old_size, new_size);
	parent->childs[parent->sub_count - 1] = child;
}
