/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 06:13:33 by arocca            #+#    #+#             */
/*   Updated: 2025/08/10 23:51:29 by arocca           ###   ########.fr       */
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
	(*res) = ft_strjoin_free(ctx, (*res), key);
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
			i++;
	}
	s_free(ctx, node->value);
	node->value = res;
}

int match(const char *str, const char *pattern)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
		return (match(str, pattern+1) || (*str && match(str+1, pattern)));
	if (*pattern == *str)
		return (match(str+1, pattern+1));
	return (0);
}

char **expand_glob(t_ctx *ctx, const char *pattern)
{
	int				count;
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;

	count = 0;
	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return NULL;
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue;
		if (match(entry->d_name, pattern))
		{
			count++;
			matches = s_realloc(ctx, matches, sizeof(char *) * count, sizeof(char *) * (count + 1));
			matches[count - 1] = s_save(ctx, ft_strdup(entry->d_name));
			matches[count] = NULL;
		}
	}
	closedir(dir);
	return (matches);
}

void sort_char_table(char **arr)
{
	int		i;
	int		j;
	int		len;
	char	*tmp;

	i = 0;
	len = 0;
	while (arr[len])
		len++;
	while (i < len)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}


void	add_wlcd_value(t_ctx *ctx, t_ast *node, char **matches)
{
	int		i;
	t_token	*tmp;

	i = 1;
	if (!matches || !matches[0] || !node->childs)
		return ;
	s_free(ctx, node->childs[0]->value);
	node->childs[0]->value = s_save(ctx, ft_strdup(matches[0]));
	while (matches[i])
	{
		tmp = create_token(ctx, matches[i], TOKEN_WORD, NONE);
		set_merge_value(&tmp, true);
		ast_add(ctx, node, new_ast(ctx, AST_COMMAND, tmp));
		free_tokens(ctx, &tmp);
		i++;
	}
	double_free(ctx, (void **)matches, 0);
}

void merge_ast(t_ctx *ctx, t_ast *node)
{
	int	i;
	t_ast *curr;
	t_ast *next;

	i = 0;
	if (!node || !node->childs)
		return;
	while (i < node->sub_count - 1)
	{
		curr = node->childs[i];
		next = node->childs[i + 1];
		if (!curr || !next)
		{
			i++;
			continue;
		}
		if (!next->has_space)
		{
			curr->value = ft_strjoin_free(ctx, curr->value, next->value);
			next->value = NULL;
			remove_ast_child(ctx, node, i + 1);
		}
		else
			i++;
	}
}


void	expand_childs(t_ctx *ctx, t_ast *node)
{
	int		i;
	int		sub;
	t_ast	**childs;
	char	**matches;

	i = 0;
	childs = node->childs;
	sub = node->sub_count;
	while (i < sub)
	{
		if (childs[i]->type != AST_COMMAND)
			break ;
		expand_args(ctx, childs[i]);
		if (childs[i]->quote == NONE && ft_strchr(childs[i]->value, '*'))
		{
			matches = expand_glob(ctx, childs[i]->value);
			if (matches)
			{
				sort_char_table(matches);
				add_wlcd_value(ctx, node, matches);
			}
		}
		i++;
	}
	return (merge_ast(ctx, node));
}
