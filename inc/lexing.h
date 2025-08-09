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
	TOKEN_OR,			// Bonus part
	TOKEN_AND,			// Bonus part
	TOKEN_LPAR,			// Bonus part
	TOKEN_RPAR,			// Bonus part
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
	bool	merge;
}				t_lexing;

typedef struct s_token
{
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
	t_quote			quote;
	char			*value;
}	t_token;

typedef struct s_wildcards
{
	DIR				*d;
	struct dirent	*e;
	char			*pattern;
	char			*dir;
}				t_wildcards;

/* -- // Tokenisation \\ -- */
t_token	*tokenize(t_ctx *ctx, char *input);

/* -- // Allocations \\ -- */
void	free_tokens(t_ctx *ctx, t_token **list);
void	add_token(t_token **head, t_token *new);
t_token	*create_token(t_ctx *ctx, char *value, t_token_type type, t_quote quote);

/* -- // Handlers \\ -- */
void	handle_word(t_ctx *ctx, t_lexing *s, t_token **tokens);
void	handle_redir(t_ctx *ctx, t_lexing *s, t_token **tokens);
void	handle_parenthesis(t_ctx *ctx, t_lexing *s, t_token **tokens);
void	handle_bonus(t_ctx *ctx, t_lexing *s, t_token **tokens, char op);
void	handle_quotes(t_ctx *ctx, t_lexing *s, t_token **tokens, char quote);

/* -- // Utils \\ -- */
int		is_operator(char c);
t_token	*get_last_token(t_token *tokens);
void	init_s(t_lexing *s, char *input);
char	*append_char(t_ctx *ctx, char *res, char c);
char	*has_expand(t_ctx *ctx, char type, char *s);


char	*ft_strcpy(char *dst, const char *src);
char	*ft_strcat(char *dest, const char *src);

#endif

/* Utils */
// t_token	*simple_tok(t_ctx *ctx, t_lexing *lx, char **res, int len);
// bool	is_eof(t_token *token);
// void	ajust_data(t_lexing *s);
// t_token	*expand_tilde(t_ctx *ctx, t_lexing *lx, char **s, char **res);
// t_token	*expand_args(t_ctx *ctx, t_token *tokens, t_lexing *lx, char *s);

/* -- Bonus - Functions -- */
// int		wildcards_match(t_ctx *ctx, const char *pat, const char *str, int wc_i);
// t_token	*expand_wildcards(t_ctx *ctx, const char *pattern);
// void	expand_last_token_if_needed(t_ctx *ctx, t_token **tokens);
// int		count_wildcards(char *input);
// void	init_is_quote(t_ctx *ctx, char *input);