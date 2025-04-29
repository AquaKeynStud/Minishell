/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_chdir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 07:57:03 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 10:48:58 by abouclie         ###   ########.fr       */
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

// Ajoute une variable à ta t_env
t_env *add_env(t_env *env, const char *key, const char *value) {
	t_env *new = malloc(sizeof(t_env));
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = env;
	return new;
}

// Affiche tout l'env
void print_env(t_env *env) {
	while (env) {
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int	update_env(t_env *env, const char *key, const char *value)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (0);
		}
		env = env->next;
	}
	return (1); // non trouvé
}

char *get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
			env = env->next;
		}
		return (NULL); // pas trouvé
	}
	
	int	ft_cd(char **args, t_env *env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	*path;

	path = args[1];
	if (args[2])
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	if (!path)
	path = get_env_value(env, "HOME");
	if (args[2])
	{
		ft_printf("cd: HOME is not set\n");
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd");
		return (1);
	}
	oldpwd = ft_strdup(cwd);
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd (after chdir)");
		free(oldpwd);
		return (1);
	}
	update_env(env, "OLDPWD", oldpwd);
	update_env(env, "PWD", cwd);
	free(oldpwd);
	return (0);
}

// --- main de test ---
int main(void) {
	t_env *env = NULL;

	// Simule un env avec PWD, OLDPWD, HOME
	env = add_env(env, "PWD", "/");
	env = add_env(env, "OLDPWD", "/old");
	env = add_env(env, "HOME", "/tmp");

	printf("Env initial:\n");
	print_env(env);

	// Test 1: cd sans argument (va vers HOME)
	printf("\nTest: cd sans argument\n");
	char *args1[] = {"cd", NULL};
	ft_cd(args1, env);
	print_env(env);

	// Test 2: cd vers un path existant
	printf("\nTest: cd vers /usr\n");
	char *args2[] = {"cd", "/usr", NULL};
	ft_cd(args2, env);
	print_env(env);

	// Test 3: get_env_value
	printf("\nTest: get_env_value(\"PWD\") = %s\n", get_env_value(env, "PWD"));

	// Test 4: update_env
	printf("\nTest: update_env(\"HOME\", \"/home/test\")\n");
	update_env(env, "HOME", "/home/test");
	print_env(env);

	return 0;
}