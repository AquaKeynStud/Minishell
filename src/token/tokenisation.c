/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:37:59 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/30 18:08:18 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <stdbool.h>

static void	handle_redirection(const char *input, int *i, t_token **tokens)
{
	char	op;
	char	*str;
	int		len;
	t_token	*token;

	len = 1;
	op = input[*i];
	if (input[*i + 1] == op)
		len = 2;
	str = ft_strndup(&input[*i], len);
	if (!str)
		return ;
	if (op == '>' && len == 2)
		token = create_token(str, TOKEN_REDIR_APPEND);
	else if (op == '<' && len == 2)
		token = create_token(str, TOKEN_HEREDOC);
	else if (op == '>')
		token = create_token(str, TOKEN_REDIR_OUT);
	else
		token = create_token(str, TOKEN_REDIR_IN);
	if (token)
		add_token(tokens, token);
	free(str);
	*i += len;
}

/* Récupère le dernier token de la liste */
static t_token *get_last_token(t_token *tokens)
{
    if (!tokens)
        return NULL;
    while (tokens->next)
        tokens = tokens->next;
    return tokens;
}

/* Concatène new_str à last->content (qui doit être TOKEN_WORD) */
static void merge_word_token(t_token *last, const char *new_str)
{
    size_t old_len = ft_strlen(last->value);
    size_t add_len = ft_strlen(new_str);
    char *merged = malloc(old_len + add_len + 1);
    if (!merged)
        return;  /* en prod : gérer l’erreur */
    ft_memcpy(merged, last->value, old_len);
    ft_memcpy(merged + old_len, new_str, add_len + 1);
    free(last->value);
    last->value = merged;
}

/* Si le dernier token est un mot, on le fusionne ; sinon on en crée un nouveau */
static void add_or_merge_word(t_token **tokens, char *content, bool merge_allowed)
{
    if (merge_allowed)
    {
        t_token *last = get_last_token(*tokens);
        if (last && last->type == TOKEN_WORD)
        {
            merge_word_token(last, content);
            free(content);
            return;
        }
    }
    /* sinon on crée un nouveau token */
    t_token *tok = create_token(content, TOKEN_WORD);
    if (tok)
        add_token(tokens, tok);
    /* create_token prend possession de content */
}

static void handle_quotes(const char *input, int *i, t_token **tokens, bool merge_allowed)
{
    int   start;
    char  quote;

    quote = input[*i];
    (*i)++;
    start = *i;
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
        return;  /* quote non fermée : on pourrait gérer l’erreur */
    int len = *i - start;
    char *content = ft_strndup(&input[start], len);
    if (content)
        add_or_merge_word(tokens, content, merge_allowed);
    (*i)++;
}

static void handle_word(const char *input, int *i, t_token **tokens, bool merge_allowed)
{
    int start = *i;
    while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '"')
            break;
        (*i)++;
    }
    int len = *i - start;
    if (len > 0)
    {
        char *str = ft_strndup(&input[start], len);
        if (str)
            add_or_merge_word(tokens, str, merge_allowed);
    }
}

t_token *tokenize(const char *input)
{
    t_token *tokens = NULL;
    int      i = 0;
    bool     merge_allowed = false;  // au départ, pas d’agrégation

    while (input[i])
    {
        if (is_whitespace(input[i]))
        {
            // dès qu’on voit un espace : on sacrifie la fusion
            i++;
            merge_allowed = false;
            continue;
        }

        if (input[i] == '\'' || input[i] == '"')
        {
            handle_quotes(input, &i, &tokens, merge_allowed);
            // après un quote, si le prochain token est mot/quote sans séparateur, 
            // on l’agrégera
            merge_allowed = true;
        }
        else if (input[i] == '|')
        {
            add_token(&tokens, create_token("|", TOKEN_PIPE));
            i++;
            merge_allowed = false;
        }
        else if (input[i] == '>' || input[i] == '<')
        {
            handle_redirection(input, &i, &tokens);
            merge_allowed = false;
        }
        else
        {
            handle_word(input, &i, &tokens, merge_allowed);
            merge_allowed = true;
        }
    }

    return tokens;
}
