/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 08:47:13 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/11 12:54:53 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*find_env_node(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_or_update_env(t_ctx *ctx, t_env **env, char *key, char *value)
{
	t_env	*existing;
	t_env	*new_node;

	existing = find_env_node(*env, key);
	if (existing)
	{
		if (value)
		{
			s_free(ctx, existing->value);
			existing->value = s_save(ctx, ft_strdup(value));
		}
	}
	else
	{
		new_node = create_env_node(ctx, key, value);
		if (!new_node)
			return ;
		append_env_node(env, new_node);
	}
}

t_env	*create_env_node(t_ctx *ctx, const char *key, const char *value)
{
	t_env	*new;

	new = s_malloc(ctx, sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = s_save(ctx, ft_strdup(key));
	if (!new->key)
	{
		s_free(ctx, new);
		return (NULL);
	}
	if (value)
		new->value = s_save(ctx, ft_strdup(value));
	else
		new->value = NULL;
	if (value && !new->value)
	{
		s_free(ctx, new->key);
		s_free(ctx, new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

void	append_env_node(t_env **env, t_env *new_node)
{
	t_env	*current;

	if (!*env)
		*env = new_node;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}
