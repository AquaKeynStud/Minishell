/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:37:36 by arocca            #+#    #+#             */
/*   Updated: 2025/05/01 11:11:34 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexing.h"
#include <stdbool.h>

static void	handle_redir(char *input, int *i, t_token **tokens, bool *merge)
{
	char	op;
	int		len;
	char	*str;
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
	*merge = false;
}

static void	handle_quotes(char *input, int *i, t_token **tokens, bool *merge)
{
	int		len;
	int		start;
	char	quote;
	char	*content;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
		return ;
	len = *i - start;
	content = ft_strndup(&input[start], len);
	if (content)
		add_or_merge_word(tokens, content, *merge);
	(*i)++;
	*merge = true;
}

static void	handle_pipe(int *i, t_token **tokens, bool *merge)
{
	add_token(tokens, create_token("|", TOKEN_PIPE));
	(*i)++;
	*merge = false;
}

static void	handle_word(char *input, int *i, t_token **tokens, bool *merge)
{
	int		len;
	char	*str;
	int		start;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			break ;
		(*i)++;
	}
	len = *i - start;
	if (len > 0)
	{
		str = ft_strndup(&input[start], len);
		if (str)
			add_or_merge_word(tokens, str, *merge);
	}
	*merge = true;
}

t_token	*tokenize(char *input)
{
	int		i;
	t_token	*tokens;
	bool	merge;

	i = 0;
	tokens = NULL;
	merge = false;
	while (input[i])
	{
		if (is_whitespace(input[i]))
		{
			i++;
			merge = false; // dès qu’on voit un espace : on sacrifie la fusion
		}
		if (input[i] == '\'' || input[i] == '"')
			handle_quotes(input, &i, &tokens, &merge);
		else if (input[i] == '|')
			handle_pipe(&i, &tokens, &merge);
		else if (input[i] == '>' || input[i] == '<')
			handle_redir(input, &i, &tokens, &merge);
		else
			handle_word(input, &i, &tokens, &merge);
	}
	return (tokens);
}
