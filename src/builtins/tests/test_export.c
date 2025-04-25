/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:40:39 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/25 12:57:40 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

// ------------ FONCTIONS D'UTILS POUR TEST --------------------
t_env *create_env_node(char *key, char *value)
{
	t_env *node = malloc(sizeof(t_env));
	if (!node)
		return NULL;  // Gère l'échec de l'allocation de mémoire

	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return NULL;  // Gère l'échec de l'allocation de la clé
	}

	node->value = value ? ft_strdup(value) : NULL;
	if (value && !node->value)
	{
		free(node->key);
		free(node);
		return NULL;  // Gère l'échec de l'allocation de la valeur
	}

	node->next = NULL;
	return node;
}


void free_env(t_env *env)
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

void	print_env(t_env *env)
{
	while (env)
	{
		ft_printf("%s=%s\n", env->key, env->value ? env->value : "(null)");
		env = env->next;
	}
}

void	append_env_node(t_env **env, t_env *new_node)
{
	if (!*env)
	{
		*env = new_node;
		return;
	}
	t_env *tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

// ------------ MAIN DE TEST --------------------
int	main(void)
{
	t_env *env = NULL;

	append_env_node(&env, create_env_node("PATH", "/usr/bin:/bin"));
	append_env_node(&env, create_env_node("USER", "student42"));
	append_env_node(&env, create_env_node("HOME", "/home/user"));
	append_env_node(&env, create_env_node("EMPTY", NULL));

	char *args2[] = { "export", "TEST=abouclie", NULL};
	char *args1[] = { "export", NULL };
	
	ft_export(args2, &env);

	ft_export(args1, &env);

	free_env(env);
	return 0;
}

// cc src/builtins/ft_export.c src/builtins/tests/test_export.c src/builtins/utils_builtins.c -Iinc -I./libft -L./libft -lft