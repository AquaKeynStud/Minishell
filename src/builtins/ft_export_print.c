/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:44:49 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/11 12:57:37 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static void	swap_env_value(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = a->key;
	tmp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_value;
}

static void	sort_env(t_env **env)
{
	t_env	*current;
	int		sorted;

	if (!env || !(*env))
		return ;
	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		current = *env;
		while (current && current->next)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_env_value(current, current->next);
				sorted = 0;
			}
			current = current->next;
		}
	}
}

static t_env	*copy_env_list(t_ctx *ctx, t_env *env)
{
	t_env	*copy;

	copy = NULL;
	while (env)
	{
		append_env_node(&copy, create_env_node(ctx, env->key, env->value));
		env = env->next;
	}
	return (copy);
}

void	print_sorted_env(t_ctx *ctx, t_env *env)
{
	t_env	*copy;
	t_env	*tmp;

	copy = copy_env_list(ctx, env);
	sort_env(&copy);
	tmp = copy;
	while (tmp)
	{
		if (tmp->value)
			ft_printf("export %s=\"%s\"\n", tmp->key, tmp->value);
		else
			ft_printf("export %s\n", tmp->key);
		tmp = tmp->next;
	}
	free_env(ctx, &copy);
}
