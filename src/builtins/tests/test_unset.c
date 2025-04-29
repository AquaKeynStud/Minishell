/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:10:55 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/29 11:41:10 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static int	is_option(char **args)
{
	char	*perror_msg;
	int	i;
	
	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-')
		{
			ft_printf("unset: %s: no option allowed\n", args[i]);
			return (2);
		}
		i++;
	}
	return (0);
}

void	remove_env_var(char *key, t_env **env)
{
	t_env	*tmp;
	t_env	*prev;
	
	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	ft_unset(char **args, t_env *env)
{
	int		i;
	int		exit_code;

	i = 1;
	exit_code = is_option(args);
	while (args[i])
	{
		remove_env_var(args[i], &env);
		i++;
	}
	return (exit_code);
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

void	print_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int main(int argc, char ** argv, char **envp)
{
	t_env	*env;
	int		result;

	env = init_env(envp);
	result = ft_unset(argv, env);
	ft_printf("\n=== Environnement après unset ===\n");
	print_env(env);
	return (result);
}
// A voir si on doit gérer les variables qui sont en read only
// readonly VAR=test
// unset VAR

// bash: unset: VAR: cannot unset: readonly variable
// echo $? = 1
