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
	t_quote			quote;
	char			*value;
	struct s_ast	**childs;
	bool			has_space;
	int				sub_count;
}				t_ast;

/* -- Functions -- */
t_ast	*parse_input(t_ctx *ctx, t_token *tokens);
int		parsing_err(t_ctx *ctx, const char *msg, int code);
int		parse_redirs(t_ctx *ctx, t_ast **cmd, t_token **curr);

/* -- Allocations - Functions -- */
void	*free_ast(t_ctx *ctx, t_ast *node);
t_ast	*new_ast(t_ctx *ctx, t_ast_type type, t_token *curr);
void	remove_ast_child(t_ctx *ctx, t_ast *parent, int index);
void	*double_free_ast(t_ctx *ctx, t_ast *left, t_ast *right);
void	ast_add(t_ctx *ctx, t_ast *parent, t_ast *child, bool in_first);

/* -- Bonus - Functions -- */
bool	is_binary_op(t_token_type type);
int		syntax_error(t_ctx *ctx, t_token *tokens);
t_ast	*parse_logical(t_ctx *ctx, t_token **curr);
int		err_redir(t_ctx *ctx, t_token *tmp, t_token **curr);
void	redir_priority(t_ctx *ctx, t_ast **cmd, t_ast *redir);
t_ast	*overwrite_stub(t_ctx *ctx, t_token **curr, t_ast **cmd);
void	cat_empty_heredoc(t_ctx *ctx, t_ast **cmd, t_token *tmp);
bool	parse_parenthesis(t_ctx *ctx, t_token **curr, t_ast **cmd);

#endif