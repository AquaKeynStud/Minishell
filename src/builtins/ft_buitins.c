/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buitins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:11:12 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/18 12:52:11 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[1] && !strcmp(args[1], "-n")) // -n supprime le retour a la ligne a la fin de la chaine de caractere
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

int	ft_cd(char **args, t_env *env)
{
	char	*path;

	path = args[1];
	if (!path)
		path = get_env_value(env, "HOME");  // à implémenter
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	// Mettre à jour PWD dans env
	return (0);
}
