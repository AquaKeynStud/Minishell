/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:04:09 by arocca            #+#    #+#             */
/*   Updated: 2025/07/12 20:17:58 by arocca           ###   ########.fr       */
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
	AST_OR,			// Bonus part
	AST_AND,		// Bonus part
	AST_SUB,		// Bonus part
	AST_PIPE,
	AST_REDIR,
	AST_COMMAND,
}			t_ast_type;

typedef struct s_ast
{
	int				fd;
	t_ast_type		type;
	char			*value;
	char			*quotes;
	struct s_ast	**childs;
	int				sub_count;
	char			*wildcards;
}				t_ast;

/* -- Functions -- */
t_ast	*parse_input(t_ctx *ctx, t_token *tokens);
int		parsing_err(t_ctx *ctx, const char *msg, int code);
int		parse_redirs(t_ctx *ctx, t_ast **cmd, t_token **curr);

void	*free_ast(t_ctx *ctx, t_ast *node);
void	ast_add(t_ctx *ctx, t_ast *parent, t_ast *child);
void	*double_free_ast(t_ctx *ctx, t_ast *left, t_ast *right);
t_ast	*new_ast(t_ctx *ctx, t_ast_type type, t_token *curr);

/* -- Bonus - Functions -- */
bool	is_binary_op(t_token_type type);
int		syntax_error(t_ctx *ctx, t_token *tokens);
t_ast	*parse_logical(t_ctx *ctx, t_token **curr);
bool	check_parenthesis(t_ctx *ctx, t_token *tokens);
bool	parse_parenthesis(t_ctx *ctx, t_token **curr, t_ast **cmd);

#endif