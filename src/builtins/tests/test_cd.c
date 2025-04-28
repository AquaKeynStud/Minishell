/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 07:57:03 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/28 14:29:41 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>   // pour chdir() et getcwd()
#include <stdlib.h>   // pour exit()
#include <errno.h>    // pour errno
#include <string.h>   // pour strerror()
#include <limits.h> // pour PATH_MAX

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*node;
	char	**split;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], '=');
		node = (t_env *)malloc(sizeof(t_env));
		node->key = ft_strdup(split[0]);
		if (split[1])
			node->value = ft_strdup(split[1]);
		else
			node->value = ft_strdup("");
		node->next = env_list;
		env_list = node;
		free_split(split);
		i++;
	}
	return (env_list);
}

int main(int argc, char **argv, char **envp)
{
    t_env *env;

    (void)argc; // si tu ne les utilises pas

    env = init_env(envp);

    if (!env)
    {
        printf("Erreur d'initialisation de l'environnement\n");
        return (1);
    }

    ft_cd(argv, env);

    return 0;
}
