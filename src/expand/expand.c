/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 06:13:33 by arocca            #+#    #+#             */
/*   Updated: 2025/08/14 22:11:20 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parsing.h"
#include "minishell.h"

void	append(t_ctx *ctx, t_ast *node, char **res, int *i)
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
			i += 1 + expand_env(ctx, node, &res, (i + 1));
		else
			append(ctx, node, &res, &i);
	}
	s_free(ctx, node->value);
	node->value = res;
}

bool	check_parent(t_ctx *ctx, t_ast *ast)
{
	if ((!ast->value || !*ast->value) && ast->quote == NONE)
	{
		if (!ast->childs || ast->sub_count == 0)
			return (false);
		s_free(ctx, ast->value);
		ast->value = s_save(ctx, ft_strdup(ast->childs[0]->value));
		ast->quote = ast->childs[0]->quote;
		remove_ast_child(ctx, ast, 0);
		expand_childs(ctx, ast);
		return (true);
	}
	return (true);
}

t_ast	*expand_childs(t_ctx *ctx, t_ast *node)
{
	int		i;
	int		sub;
	t_ast	**childs;

	i = 0;
	expand_args(ctx, node);
	if (!check_parent(ctx, node))
		return (NULL);
	split_ifs(ctx, NULL, node);
	childs = node->childs;
	sub = node->sub_count;
	while (childs && i < sub)
	{
		if (!childs[i] || childs[i]->type != AST_COMMAND)
			break ;
		expand_args(ctx, childs[i]);
		if (childs[i] && (!childs[i]->value || !*childs[i]->value) && !childs[i]->has_space)
		{
			if ((i + 1) < sub && childs[i]->has_space && !childs[i + 1]->has_space)
				childs[i + 1]->has_space = childs[i]->has_space;
			remove_ast_child(ctx, node, i);
			childs = node->childs;
			sub = node->sub_count;
			continue ;
		}
		split_ifs(ctx, node, childs[i]);
		childs = node->childs;
		sub = node->sub_count;
		i++;
	}
	merge_ast(ctx, node);
	glob_ast(ctx, node);
	return (node);
}
