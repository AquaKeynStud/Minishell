/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:34:52 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/11 14:19:44 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

# include "libft.h"
# include <stddef.h>
# include <stdbool.h>
# include "minishell.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_lexing
{
	int		i;
	char	*str;
	bool	merge;
	bool	is_var;
	bool	quoted;
	int		end_quote;
}				t_lexing;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

/* Tokenisation */
t_token	*tokenize(t_ctx *ctx, char *input, bool is_var);

/* Add token */
void	free_tokens(t_ctx *ctx, t_token **list);
void	add_token(t_token **head, t_token *new);
t_token	*simple_tok(t_ctx *ctx, t_lexing *lx, char **res, int len);
t_token	*create_token(t_ctx *ctx, const char *value, t_token_type type);

/* Utils */
int		is_operator(char c);
bool	is_eof(t_token *token);
void	ajust_data(t_lexing *s);
t_token	*get_last_token(t_token *tokens);
char	*append_char(t_ctx *ctx, char *res, char c);
void	init_s(t_lexing *s, char *input, bool is_var);
t_token	*expand_tilde(t_ctx *ctx, t_lexing *lx, char **s, char **res);
t_token	*expand_args(t_ctx *ctx, t_token *tokens, t_lexing *lx, char *s);
bool	add_or_merge(t_ctx *ctx, t_token **tok, t_lexing *lx, t_token *content);

#endif