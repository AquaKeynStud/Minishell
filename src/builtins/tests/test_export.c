/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:40:39 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/28 11:10:51 by abouclie         ###   ########.fr       */
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

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*node;
	char	**split;
	t_env	*env_list;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], '='); // "VAR=VALUE"
		node = (t_env *)malloc(sizeof(t_env));
		node->key = ft_strdup(split[0]);
		if (split[1])
			node->value = ft_strdup(split[1]);
		else
			node->value = ft_strdup(""); // ou NULL
		node->next = env_list;
		env_list = node;
		// double_free((void **)split, 0);
		i++;
	}
	return (env_list);
}

// ------------ MAIN DE TEST --------------------
int	main(int argc, char **argv, char **envp)
{
	t_env	*env = NULL;

	env = init_env(envp);
	ft_export(argv, &env);


	char	*args2[2];
	args2[0] = "export";
	args2[1] = NULL;
	ft_export(args2, &env);

	return 0;
}

// cc src/builtins/ft_export.c src/builtins/ft_export_utils.c src/builtins/tests/test_export.c src/builtins/utils_builtins.c -Iinc -I./libft -L./libft -lft