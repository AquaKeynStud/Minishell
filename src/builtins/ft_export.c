/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:32:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/02 19:29:18 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "minishell.h"

static void	add_or_update_env(t_env **env, char *key, char *value)
{
	char	*existing;
	t_env	*new_node;

	existing = get_from_env(*env, key);
	if (existing)
	{
		free(existing);
		if (value)
			existing = ft_strdup(value);
		else
			existing = NULL;
	}
	else
	{
		new_node = create_env_node(key, value);
		if (!new_node)
			return ;
		append_env_node(env, new_node);
	}
}

static int	parse_env_assignment(char *arg, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	else
	{
		*key = ft_substr(arg, 0, equal_pos - arg);
		*value = ft_strdup(equal_pos + 1);
	}
	if (!(*key))
		return (0);
	return (1);
}

int	process_env_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		exit_code;

	key = NULL;
	value = NULL;
	exit_code = 0;
	if (!parse_env_assignment(arg, &key, &value))
		return (1);
	if (!key || key[0] == '\0')
	{
		ft_dprintf(2, "export: '%s': not a valid identifier\n", arg);
		exit_code = 1;
	}
	else
	{
		exit_code = is_valid_key(key);
		if (exit_code == 0)
			add_or_update_env(env, key, value);
	}
	free(key);
	free(value);
	return (exit_code);
}

int	ft_export(char **args, t_env **env)
{
	int	arg_count;
	int	i;
	int	exit_code;

	exit_code = 0;
	arg_count = count_args(args);
	if (arg_count == 1)
	{
		print_sorted_env(*env);
		return (0);
	}
	i = 1;
	while (i < arg_count)
	{
		exit_code = process_env_arg(args[i], env);
		i++;
	}
	free(args);
	return (exit_code);
}
