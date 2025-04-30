/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 09:07:51 by arocca            #+#    #+#             */
/*   Updated: 2025/04/30 09:46:42 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include "parsing.h"
#include "minishell.h"

/*
** new_ast : Create a new AST node.
** @type: node type (AST_COMMAND, AST_PIPE, AST_REDIRECTION)
** @value: value to store (command, pipe, etc.)
** Return a pointer to the new allocated node or NULL if it fails.
*/
t_ast	*new_ast(t_ast_type type, const char *value)
{
	t_ast	*node;

	node = (t_ast *)s_malloc(sizeof(t_ast));
	node->type = type;
	node->value = NULL;
	if (value)
		node->value = ft_strdup(value);
	node->fd = -1;
	node->sub_count = 0;
	node->childs = NULL;
	return (node);
}

/*
** ast_add_child : Add the given  child to the given parent.
** @parent: The parent node/ast.
** @child: The child node you want to add.
** Reallocate the table of childs and update sub_count.
*/
void	ast_add_child(t_ast *parent, t_ast *child)
{
	size_t	old_size;
	size_t	new_size;

	old_size = sizeof(t_ast *) * parent->sub_count;
	new_size = sizeof(t_ast *) * (++parent->sub_count);
	parent->childs = s_realloc(parent->childs, old_size, new_size);
	parent->childs[parent->sub_count - 1] = child;
}

/*
** free_ast : Free the Abstract Syntax Tree recursively.
** @node : The root of the ast to free.
** Return NULL
*/
void	*free_ast(t_ast *node)
{
	int	i;

	if (!node)
		return (NULL);
	i = 0;
	while (i < node->sub_count)
	{
		free_ast(node->childs[i]);
		i++;
	}
	free(node->childs);
	free(node->value);
	free(node);
	return (NULL);
}

/*
** free_ast : Free the left and the right ast of a pipe recursively.
** @left : The root of the first ast to free.
** @right : The root of the second ast to free.
** Return NULL
*/
void	*double_free_ast(t_ast *left, t_ast *right)
{
	free_ast(left);
	free_ast(right);
	return (NULL);
}
