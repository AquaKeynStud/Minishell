/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:49 by arocca            #+#    #+#             */
/*   Updated: 2025/04/18 10:42:30 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h" // Pour strlen
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "readline.h"

static bool	is_only_whitespaces(char *input)
{
	while (*input && (*input == ' ' || *input == '\t')) // On incrémente le pointeur tant qu'il pointe un whitespace
		input++;
	if (*input) // Si on est pas à la fin d'input
		return (false);
	return (true);
}

static void	trim_input(char **input)
{
	int		len; // pour stocker la taille dela chaine trimmée
	int		start; // pour conter les whitespaces en début de commande
	char	*trimmed; // La nouvelle chaine trimmée

	start = 0;
	if (!(*input)) // Si pas d'imput, on ne fait rien
		return ;
	while ((*input)[start] == ' ' || (*input)[start] == '\t') // Tant qu'il y a des whitespaces, on déplace le début
		start++;
	len = ft_strlen(*input); // On met len à la fin de la chaine
	while ((len > start) && ((*input)[len - 1] == ' ' || (*input)[len - 1] == '\t')) // Tant qu'il y a des whitespaces, on déplace la fin
		len--;
	len -= start; // On enleve les whitespaces du début à la taille finale de la chaine
	trimmed = (char *)malloc(sizeof(char) * (len + 1));
	if (!trimmed || len <= 0) // Si len vaut 0, on va free dans la boucle principale
		return ;
	trimmed[len] = '\0';
	while (len--)
        trimmed[len] = (*input)[start + len]; // On copie input dans trimmed a partir de la fin
	free(*input);
	*input = trimmed; // input devient la nouvelle chaine créée
}

void	get_input_loop(void)
{
	char	*input;
	
	while (1)
	{
		input = readline("\033[1m\033[32mminishell \033[31m➤ \033[0m");
		if (!input)
			break ;
		if (*input && !is_only_whitespaces(input)) // On ajoute la chaine a l'historique si elle n'est pas vide ou seulement des whitespaces
			add_history(input);
		trim_input(&input); // On enleve les espaces au début et à la fin
		// printf("Nouvelle commande : [%s]\n", input); // Pour voir la commande enregistrée trimmée
		free(input);
	}
	rl_clear_history();
}
