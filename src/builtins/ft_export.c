/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:32:23 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/11 12:58:21 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "minishell.h"

static int	is_valid_key(char *key, char *arg)
{
	int	i;

	i = 0;
	if (key[0] == '-' && key[1])
	{
		ft_dprintf(2, "minishell: export: ");
		ft_dprintf(2, "%c%c: invalid option\n", key[0], key[1]);
		return (2);
	}
	if (ft_isdigit(key[i]))
	{
		ft_dprintf(2, "export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	while (key[i])
	{
		if (!in_str(key[i], "_", true))
		{
			ft_dprintf(2, "export: `%s': not a valid identifier\n", arg);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	parse_env_set(t_ctx *ctx, char *arg, char **key, char **value)
{
	char	*pos;
	int		sep;
	t_env	**env;

	pos = ft_strchr(arg, '=');
	sep = pos - arg;
	env = &ctx->env;
	if (!pos)
	{
		*key = s_save(ctx, ft_strdup(arg));
		*value = NULL;
	}
	else if (sep >= 2 && arg[sep - 1] == '+' && in_str(arg[sep - 2], "_", 1))
	{
		*key = ft_substr(arg, 0, pos - arg - 1);
		*value = s_save(ctx, ft_strjoin(get_from_env(*env, *key), pos + 1));
	}
	else
	{
		*key = ft_substr(arg, 0, pos - arg);
		*value = s_save(ctx, ft_strdup(pos + 1));
	}
	if (!(*key))
		return (0);
	return (1);
}

static int	process_env_arg(t_ctx *ctx, char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		exit_code;

	key = NULL;
	value = NULL;
	exit_code = 0;
	if (!parse_env_set(ctx, arg, &key, &value))
		return (1);
	exit_code = is_valid_key(key, arg);
	if (!key || key[0] == '\0')
	{
		ft_dprintf(2, "export: `");
		ft_dprintf(2, "%s': not a valid identifier\n", arg);
		exit_code = 1;
	}
	else if (exit_code == 0)
		add_or_update_env(ctx, env, key, value);
	s_free(ctx, key);
	s_free(ctx, value);
	return (exit_code);
}

int	ft_export(t_ctx *ctx, char **args, t_env **env)
{
	int	arg_count;
	int	i;
	int	exit_code;

	exit_code = 0;
	arg_count = count_args(args);
	if (arg_count == 1)
	{
		print_sorted_env(ctx, *env);
		s_free(ctx, args);
		return (0);
	}
	i = 1;
	while (i < arg_count)
	{
		exit_code = process_env_arg(ctx, args[i], env);
		i++;
	}
	s_free(ctx, args);
	return (exit_code);
}
