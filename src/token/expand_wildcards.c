/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 01:01:46 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/23 03:05:37 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

static void	process_entry(t_ctx *ctx, t_wildcards *wildcards, t_token **tokens);

static char	*next_slash(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '/')
		i++;
	return ((char *)(s + i));
}

static void	explore_wildcards(t_ctx *ctx, char *pattern, char *dir, t_token **tokens)
{
	t_wildcards	wildcards;

	wildcards.d = opendir(dir);
	wildcards.pattern = pattern;
	wildcards.dir = dir;
	if (!wildcards.d)
		return ;
	while ((wildcards.e = readdir(wildcards.d)) != NULL)
		process_entry(ctx, &wildcards, tokens);
	closedir(wildcards.d);
}

static void	process_entry(t_ctx *ctx, t_wildcards *wildcards, t_token **tokens)
{
	char		path[1024];
	char		part[256];
	char		*slash;
	struct stat	st;
	
	if (!ft_strcmp(wildcards->e->d_name, ".") || !ft_strcmp(wildcards->e->d_name, ".."))
		return ;
	slash = next_slash(wildcards->pattern);
	ft_strncpy(part, wildcards->pattern, slash - wildcards->pattern);
	part[slash - wildcards->pattern] = '\0';
	if (wildcards->e->d_name[0] == '.' && part[0] != '.')
		return ;
	if (!wildcards_match(ctx, part, wildcards->e->d_name, 0))
		return ;
	if (ft_strcmp(wildcards->dir, ".") == 0)
		ft_strcpy(path, wildcards->e->d_name);
	else
	{
		ft_strcpy(path, wildcards->dir);
		ft_strcat(path, "/");
		ft_strcat(path, wildcards->e->d_name);
	}
	if (*slash == '/')
	{
		if (!stat(path, &st) && S_ISDIR(st.st_mode))
			explore_wildcards(ctx, slash + 1, path, tokens);
	}
	else if (*slash == '\0')
		add_token(tokens, create_token(ctx, path, TOKEN_WORD));

}

t_token	*expand_wildcards(t_ctx *ctx, const char *pattern)
{
	t_token	*tokens;

	tokens = NULL;
	explore_wildcards(ctx, (char *)pattern, ".", &tokens);
	if (!tokens)
		tokens = create_token(ctx, pattern, TOKEN_WORD);
	ctx->has_wildcard = false;
	return (tokens);
}
