/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:56:01 by arocca            #+#    #+#             */
/*   Updated: 2025/09/04 08:07:07 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

void	replace_char(t_ast *node)
{
	int	i;

	i = 0;
	while (node->value[i])
	{
		if (node->value[i] == -1)
			node->value[i] = '*';
		i++;
	}
}

static void	append(t_ctx *ctx, t_ast *node, char **res, int *i)
{
	int	len;

	len = ft_strlen((*res));
	(*res) = s_realloc(ctx, (*res), len, len + 2);
	(*res)[len] = node->value[(*i)++];
	(*res)[len + 1] = '\0';
}

int	expand_tilde(t_ctx *ctx, char **res)
{
	int		len;
	int		t_len;
	char	*home;

	home = get_from_env(ctx->env, "HOME");
	if (home)
	{
		len = ft_strlen(home);
		t_len = ft_strlen(*res);
		*res = s_realloc(ctx, *res, t_len, t_len + len + 1);
		ft_memmove(*res + t_len, home, len);
		(*res)[t_len + len] = '\0';
	}
	return (0);
}

int	expand_env(t_ctx *ctx, t_ast *node, char **res, int i)
{
	int		len;
	char	*key;

	if (node->value[i] == '?')
	{
		key = s_save(ctx, ft_itoa(ctx->status));
		(*res) = ft_strjoin_free(ctx, (*res), key);
		return (1);
	}
	key = s_save(ctx, ft_strdup(""));
	while (in_str(node->value[i], "?_", true))
		append(ctx, node, &key, &i);
	len = ft_strlen(key);
	key = s_save(ctx, ft_strdup(get_from_env(ctx->env, key)));
	if (key && *key)
		(*res) = ft_strjoin_free(ctx, (*res), key);
	else
		s_free(ctx, key);
	return (len);
}

void	expand_args(t_ctx *ctx, t_ast *node)
{
	int		i;
	char	*res;

	if (!node || !node->value)
		return ;
	i = 0;
	res = s_save(ctx, ft_strdup(""));
	while (node->value[i])
	{
		if (!in_str(node->value[i], "~$", false))
			append(ctx, node, &res, &i);
		else if (node->value[i] == '~' && node->quote == NONE)
			i += 1 + expand_tilde(ctx, &res);
		else if (node->value[i] == '$' && node->quote != SINGLE)
		{
			if (!in_str(node->value[i + 1], "?_", true))
				append(ctx, node, &res, &i);
			else
				i += 1 + expand_env(ctx, node, &res, (i + 1));
		}
		else
			append(ctx, node, &res, &i);
	}
	s_free(ctx, node->value);
	node->value = res;
}
