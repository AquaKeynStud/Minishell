/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:20:27 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 11:24:19 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

int	ft_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			ft_printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

// 🔧 petite fonction pour créer un nœud d'env
t_env	*create_env_node(char *key, char *value)
{
	t_env *node = malloc(sizeof(t_env));
	if (!node)
		return NULL;
	node->key = ft_strdup(key);
	node->value = value ? ft_strdup(value) : NULL;
	node->next = NULL;
	return node;
}

// 🔧 ajoute un nœud à la fin
void	append_env_node(t_env **env, t_env *new)
{
	t_env *tmp = *env;
	if (!tmp)
	{
		*env = new;
		return;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	free_env(t_env *env)
{
	t_env *tmp;
	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

int	main(void)
{
	t_env *env = NULL;

	append_env_node(&env, create_env_node("PATH", "/usr/bin:/bin"));
	append_env_node(&env, create_env_node("HOME", "/home/user"));
	append_env_node(&env, create_env_node("SHELL", "/bin/bash"));
	append_env_node(&env, create_env_node("EMPTY", NULL)); // ne s'affichera pas

	ft_printf("Contenu de ft_env:\n");
	ft_env(env);

	free_env(env);
	return 0;
}