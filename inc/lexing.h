/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:34:52 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/01 19:17:37 by arocca           ###   ########.fr       */
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
	bool	merge;
	char	*input;
}				t_lexing;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* Tokenisation */
t_token	*tokenize(t_ctx *ctx, char *input);

/* Add token */
void	add_token(t_token **head, t_token *new);
t_token	*create_token(const char *value, t_token_type type);
void	merge_word_token(t_token *last, const char *new_str);
void	add_or_merge_word(t_token **tokens, char *content, bool merge);

/* Utils */
int		is_operator(char c);
int		is_whitespace(char c);
void	free_tokens(t_token **list);
t_token	*get_last_token(t_token *tokens);

void	init_s(t_lexing *s, char *input);
char 	*expand_args(t_ctx *ctx, char *s);

#endif