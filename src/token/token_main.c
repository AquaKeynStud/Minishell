/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:10:50 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/12 09:51:37 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

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
	const char *tests[] = {
		"cat 'mon fichier.txt' | grep \"bonjour\" >> sortie.txt",
		"echo hello > file.txt",
		"ls -l | wc -l",
		"echo \"chaine avec > dans les quotes\"",
		"commande << eof",
		"simpleword",
		"   spaced    out   ",
		"\"quote ouverte",
		"pipe | en début",
		"echo >> \"hello guys\"",
		"cat < input.txt | sort | uniq > output.txt",
		"cat /home/user/docs/file.txt",
		"echo 'bonjour le' monde",
		"ls | | wc",
		"echo \"non fermée",
		"echo hello > file1.txt >> file2.txt",
		"echo    hello   |   grep   h",
		"cat <<EOF\nline1\nline2\nEOF",
		NULL
	};
	

	for (int i = 0; tests[i] != NULL; i++)
	{
		ft_printf("Test %d: \"%s\"\n", i + 1, tests[i]);
		t_token *tokens = tokenize(tests[i]);
		print_tokens(tokens);
		ft_printf("--------------------------------------------------\n");
		// (Optionnel) Ajoute une fonction free_tokens si tu veux tout libérer proprement ici
	}
	return (0);
}


