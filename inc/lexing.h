/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:34:52 by abouclie          #+#    #+#             */
/*   Updated: 2025/08/01 11:36:08 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

# include "libft.h"
# include <stddef.h>
# include <stdbool.h>
# include <dirent.h>
# include <sys/stat.h>
# include "minishell.h"

typedef enum e_token_type
{
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_LPAR,
	TOKEN_RPAR,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_HEREDOC,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND
}			t_token_type;

typedef enum e_quote
{
	NONE,
	SINGLE,
	DOUBLE
}			t_quote;

typedef struct s_lexing
{
	int		i;
	char	*str;
	bool	has_space;
}				t_lexing;

typedef struct s_token
{
	t_token_type	type;
	t_quote			quote;
	struct s_token	*next;
	struct s_token	*prev;
	char			*value;
	bool			has_space;
}			t_token;

/* -- // Tokenisation \\ -- */
t_token	*tokenize(t_ctx *ctx, char *input);

/* -- // Allocations \\ -- */
void	free_tokens(t_ctx *ctx, t_token **list);
void	add_token(t_token **head, t_token *new);
t_token	*set_merge_value(t_token **token, bool has_space);
t_token	*new_token(t_ctx *ctx, char *value, t_token_type type, t_quote quote);

/* -- // Handlers \\ -- */
void	handle_word(t_ctx *ctx, t_lexing *s, t_token **tokens);
void	handle_redir(t_ctx *ctx, t_lexing *s, t_token **tokens);
void	handle_parenthesis(t_ctx *ctx, t_lexing *s, t_token **tokens);
void	handle_bonus(t_ctx *ctx, t_lexing *s, t_token **tokens, char op);
void	handle_quotes(t_ctx *ctx, t_lexing *s, t_token **tokens, char quote);

/* -- // Utils \\ -- */
int		is_op(char c);
t_token	*get_last_token(t_token *tokens);
void	init_s(t_lexing *s, char *input);
bool	bonus_err(t_ctx *ctx, t_token *tokens);
bool	tokens_err(t_ctx *ctx, t_token *tokens);
char	*append_char(t_ctx *ctx, char *res, char c);
char	*has_expand(t_ctx *ctx, char type, char *s);

#endif