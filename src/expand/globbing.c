/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:27:03 by arocca            #+#    #+#             */
/*   Updated: 2025/08/14 16:44:42 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int match(const char *str, const char *pattern)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
		return (match(str, pattern+1) || (*str && match(str+1, pattern)));
	if (*pattern == *str)
		return (match(str+1, pattern+1));
	return (0);
}

static void sort_char_table(char **arr)
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

static char **expand_glob(t_ctx *ctx, const char *pattern, int count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;

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

void	set_globbing(t_ctx *ctx, t_ast *parent, t_ast *child, int i)
{
	t_token	*tmp;
	char	**matches;

	if (child->quote == NONE && ft_strchr(child->value, '*'))
	{
		matches = expand_glob(ctx, child->value, 0);
		if (matches)
		{
			sort_char_table(matches);
			if (!matches || !matches[0] || !parent->childs)
				return ;
			s_free(ctx, parent->childs[0]->value);
			parent->childs[0]->value = s_save(ctx, ft_strdup(matches[0]));
			while (matches[i])
			{
				tmp = create_token(ctx, matches[i], TOKEN_WORD, NONE);
				set_merge_value(&tmp, true);
				ast_add(ctx, parent, new_ast(ctx, AST_COMMAND, tmp));
				free_tokens(ctx, &tmp);
				i++;
			}
			double_free(ctx, (void **)matches, 0);
		}
	}
}

void	glob_ast(t_ctx *ctx, t_ast *node)
{
	int	i;

	if (!node)
		return ;
	if (node->type == AST_COMMAND)
	{
		i = 0;
		while (i < node->sub_count && node->childs && node->childs[i])
		{
			set_globbing(ctx, node, node->childs[i], 1);
			i++;
		}
	}
	i = 0;
	while (i < node->sub_count && node->childs && node->childs[i])
	{
		glob_ast(ctx, node->childs[i]);
		i++;
	}
}
