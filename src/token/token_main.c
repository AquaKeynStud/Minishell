/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:10:50 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/11 08:51:03 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenisation.h"
#include "libft/libft.h"

// À adapter selon ta définition de t_token_type
const char *token_type_to_str(t_token_type type)
{
	switch (type)
	{
		case TOKEN_WORD: return "WORD";
		case TOKEN_PIPE: return "PIPE";
		case TOKEN_REDIR_IN: return "REDIR_IN";
		case TOKEN_REDIR_OUT: return "REDIR_OUT";
		case TOKEN_REDIR_APPEND: return "REDIR_APPEND";
		case TOKEN_HEREDOC: return "HEREDOC";
		default: return "UNKNOWN";
	}
}

void	print_tokens(t_token *tokens)
{
	int i = 0;

	while (tokens)
	{
		ft_printf("Token %d: type = %s, value = \"%s\"\n", i,
			token_type_to_str(tokens->type),
			tokens->value ? tokens->value : "(null)");
		tokens = tokens->next;
		i++;
	}
}

int main(void)
{
	char *line = "cat 'mon fichier.txt' | grep \"bonjour\" >> sortie.txt";
	t_token *tokens = tokenize(line);
	print_tokens(tokens);
	return (0);
}

