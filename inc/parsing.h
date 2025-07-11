/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:04:09 by arocca            #+#    #+#             */
/*   Updated: 2025/07/11 19:44:53 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* -- Includes -- */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "lexing.h"
# include <stdbool.h>
# include "minishell.h"

/* -- Structures -- */
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_REDIR,
	AST_PIPE
}			t_ast_type;

typedef struct s_ast
{
	int				fd;
	t_ast_type		type;
	char			*value;
	struct s_ast	**childs;
	int				sub_count;
}				t_ast;

/* -- Functions -- */
t_ast	*parse_input(t_ctx *ctx, t_token *tokens);
int		has_bonus_err(t_ctx *ctx, t_token *tokens);
int		parsing_err(t_ctx *ctx, const char *msg, int code);
int		parse_redirs(t_ctx *ctx, t_ast **cmd, t_token **curr);

void	*free_ast(t_ctx *ctx, t_ast *node);
void	ast_add(t_ctx *ctx, t_ast *parent, t_ast *child);
void	*double_free_ast(t_ctx *ctx, t_ast *left, t_ast *right);
t_ast	*new_ast(t_ctx *ctx, t_ast_type type, const char *value);

#endif