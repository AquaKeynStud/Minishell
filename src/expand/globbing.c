/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:27:03 by arocca            #+#    #+#             */
/*   Updated: 2025/09/04 08:22:37 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parsing.h"

static int	match(char *str, const char *pattern)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
		return (match(str, pattern + 1) || (*str && match(str + 1, pattern)));
	if (*pattern == *str || (*pattern == -1 && *str == '*'))
		return (match(str + 1, pattern + 1));
	return (0);
}

char	**append_str_to_array(t_ctx *ctx, char **arr, char *str)
{
	size_t	count;

	count = 0;
	if (arr)
		while (arr[count])
			count++;
	arr = s_realloc(ctx, arr,
			sizeof(char *) * count,
			sizeof(char *) * (count + 1 + 1));
	arr[count] = s_save(ctx, ft_strdup(str));
	arr[count + 1] = NULL;
	return (arr);
}

static char	**expand_glob(t_ctx *ctx, const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;

	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (match(entry->d_name, pattern))
			matches = append_str_to_array(ctx, matches, entry->d_name);
	}
	closedir(dir);
	return (matches);
}

void	set_globbing(t_ctx *ctx, t_ast *parent, t_ast *child)
{
	t_token	*tmp;
	int		match_i;
	char	**matches;

	matches = expand_glob(ctx, child->value);
	if (matches && matches[0] && parent->childs)
	{
		sort_char_table(matches);
		if (!(parent->type == AST_REDIR && matches[1] != NULL))
		{
			s_free(ctx, child->value);
			child->value = s_save(ctx, ft_strdup(matches[0]));
		}
		match_i = 1;
		while (matches[match_i])
		{
			tmp = new_token(ctx, matches[match_i], TOKEN_WORD, NONE);
			set_merge_value(&tmp, true);
			ast_add(ctx, parent, new_ast(ctx, AST_COMMAND, tmp), -1);
			free_tokens(ctx, &tmp);
			match_i++;
		}
		double_free(ctx, (void **)matches, 0);
	}
}

void	glob_ast(t_ctx *ctx, t_ast *node)
{
	int		i;
	t_ast	*child;

	i = 0;
	if (!node)
		return ;
	while (i < node->sub_count && node->childs && node->childs[i])
	{
		child = node->childs[i];
		if (child->quote == NONE && ft_strchr(child->value, '*'))
			set_globbing(ctx, node, child);
		i++;
	}
	i = 0;
	while (i < node->sub_count && node->childs && node->childs[i])
	{
		glob_ast(ctx, node->childs[i]);
		i++;
	}
}
