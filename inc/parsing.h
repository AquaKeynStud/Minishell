/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:04:09 by arocca            #+#    #+#             */
/*   Updated: 2025/04/18 14:00:10 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* -- Includes -- */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/* -- Structures -- */
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_REDIR,
	AST_PIPE
}			t_ast_type;

typedef struct s_ast
{
	t_ast_type		type;
	char			*value;
	struct s_ast	**childs;
	int				child_count;
}				t_ast;

/* -- Functions -- */
void	ast_add_child(t_ast *parent, t_ast *child);
t_ast	*new_ast(t_ast_type type, const char *value);

#endif