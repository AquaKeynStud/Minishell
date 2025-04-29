/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:20:27 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/29 09:46:08 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

int	ft_env(t_env *env, int argc, char **args)
{
	char	*perror_msg;
	int		i;

	i = 1;
	if (argc >= 2)
	{
		while (args[i])
		{
			if (args[i][0] == '-')
				ft_printf("env: '%s': no option allowed\n", args[i]);
			else
				ft_printf("env: '%s': no argument allowed\n", args[i]);
			i++;
		}
		return (1);
	}
	while (env)
	{
		if (env->value)
			ft_printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

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

int	main(int argc, char **argv, char **envp)
{
	t_env *env;
	int		result;

    (void)argc; // si tu ne les utilises pas

    env = init_env(envp);

    if (!env)
    {
        printf("Erreur d'initialisation de l'environnement\n");
        return (1);
    }

    result = ft_env(env, argc, argv);

    return (result);
}