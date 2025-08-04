/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 01:01:46 by abouclie          #+#    #+#             */
/*   Updated: 2025/07/24 01:29:03 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

static void	build_and_process_path(t_ctx *ctx, t_wildcards *w,
				char *slash, t_token **tokens);

static char	*next_slash(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '/')
		i++;
	return ((char *)(s + i));
}

static int	should_skip_entry(t_ctx *ctx, t_wildcards *w, char *part)
{
	char		*slash;

	if (!ft_strcmp(w->e->d_name, ".") || !ft_strcmp(w->e->d_name, ".."))
		return (1);
	slash = next_slash(w->pattern);
	ft_strncpy(part, w->pattern, slash - w->pattern);
	part[slash - w->pattern] = '\0';
	if (w->e->d_name[0] == '.' && part[0] != '.')
		return (1);
	if (!wildcards_match(ctx, part, w->e->d_name, 0))
		return (1);
	return (0);
}

void	explore_wildcards(t_ctx *ctx, char *pattern, char *dir,
			t_token **tokens)
{
	t_wildcards	wild;
	char		part[256];
	char		*slash;

	wild.pattern = pattern;
	wild.dir = dir;
	wild.d = opendir(dir);
	if (!wild.d)
		return ;
	wild.e = readdir(wild.d);
	while (wild.e != NULL)
	{
		slash = next_slash(pattern);
		if (!should_skip_entry(ctx, &wild, part))
			build_and_process_path(ctx, &wild, slash, tokens);
		wild.e = readdir(wild.d);
	}
	closedir(wild.d);
}

static void	build_and_process_path(t_ctx *ctx, t_wildcards *w, char *slash,
		t_token **tokens)
{
	char		path[1024];
	struct stat	st;

	if (ft_strcmp(w->dir, ".") == 0)
		ft_strcpy(path, w->e->d_name);
	else
	{
		ft_strcpy(path, w->dir);
		ft_strcat(path, "/");
		ft_strcat(path, w->e->d_name);
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
