/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 08:37:59 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/11 09:28:19 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenisation.h"

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

static void	handle_quotes(const char *input, int *i, t_token **tokens)
{
	char	quote;
	char	*str;
	int		start;
	int		len;
	t_token	*token;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	len = *i - start;
	str = ft_strndup(&input[start], len);
	if (str)
	{
		token = create_token(str, TOKEN_WORD);
		if (token)
			add_token(tokens, token);
		free(str);
	}
	if (input[*i] == quote)
		(*i)++;
}

static void	handle_word(const char *input, int *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*str;
	t_token	*token;

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
		{
			token = create_token(str, TOKEN_WORD);
			if (token)
				add_token(tokens, token);
			free(str);
		}
	}
}

t_token	*tokenize(const char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (is_whitespace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		if (input[i] == '\'' || input[i] == '"')
			handle_quotes(input, &i, &tokens);
		else if (input[i] == '|')
		{
			add_token(&tokens, create_token("|", TOKEN_PIPE));
			i++;
		}
		else if (input[i] == '>' || input[i] == '<')
			handle_redirection(input, &i, &tokens);
		else
			handle_word(input, &i, &tokens);
	}
	return (tokens);
}
