/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:04:09 by arocca            #+#    #+#             */
/*   Updated: 2025/04/30 11:47:58 by arocca           ###   ########.fr       */
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

void	*parsing_err(t_ctx *ctx, const char *msg, int code);

void	*free_ast(t_ast *node);
void	ast_add_child(t_ast *parent, t_ast *child);
void	*double_free_ast(t_ast *left, t_ast *right);
t_ast	*new_ast(t_ast_type type, const char *value);

void	redir_priority(t_ast **cmd, t_ast *redir);
void	cat_empty_heredoc(t_ast **cmd, t_token *tmp);
t_ast	*overwrite_stub(t_token **curr, t_ast **cmd);

void	redir_priority(t_ast **cmd, t_ast *redir);
void	cat_empty_heredoc(t_ast **cmd, t_token *tmp);
t_ast	*overwrite_stub(t_token **curr, t_ast **cmd);

#endif