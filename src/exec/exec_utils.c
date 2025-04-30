/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:04:52 by arocca            #+#    #+#             */
/*   Updated: 2025/04/30 11:39:02 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <errno.h>
#include "parsing.h"
#include "minishell.h"
#include "sigaction.h"

/**
** expand_arg: duplique la chaîne d'entrée en remplaçant les variables shell.
** - "$?" → code de retour ctx->status
** - "$VAR" → valeur de la variable d'environnement, ou chaîne vide
** - sinon    → copie brute
**/
static char	*expand_arg(t_ctx *ctx, char *s)
{
	char	*val;

	if (!s)
		return (ft_strdup(""));
	if (!ft_strcmp(s, "$?"))
		return (ft_itoa(ctx->status));
	if (s[0] == '$' && s[1])
	{
		val = get_from_env(ctx->env, s + 1);
		if (val)
			return (ft_strdup(val));
		return (ft_strdup(""));
	}
	return (ft_strdup(s));
}

char	**ast_to_argv(t_ctx *ctx, t_ast *node)
{
	int		pos;
	char	**argv;
	t_ast	**childs;

	if (!node)
		return (NULL);
	pos = node->sub_count;
	childs = node->childs;
	argv = s_malloc((pos + 2) * sizeof(char *));
	argv[pos + 1] = NULL;
	argv[0] = expand_arg(ctx, node->value);
	if (!argv[0])
		free(argv);
	while (pos > 0)
	{
		argv[pos] = expand_arg(ctx, childs[pos - 1]->value);
		if (!argv[pos])
			double_free((void **)argv, 0);
		pos--;
	}
	return (argv);
}

char	*get_path(const char *cmd, t_env *env)
{
	int		i;
	char	**paths;
	char	*raw_path;
	char	*full_cmd;

	i = 0;
	if (!cmd)
		return (NULL);
	raw_path = get_from_env(env, "PATH");
	if (ft_strchr(cmd, '/') || !raw_path)
		return (ft_strdup(cmd));
	paths = ft_split(raw_path, ':');
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_cmd = join_with_delim(paths[i++], cmd, "/");
		if (!full_cmd || !access(full_cmd, X_OK))
			break ;
		free(full_cmd);
		full_cmd = NULL;
	}
	double_free((void **)paths, 0);
	return (full_cmd);
}

int	free_cmd(char *path, char **args, char **envp, int exit_code)
{
	if (path)
		free(path);
	if (args)
		double_free((void **)args, 0);
	if (envp)
		double_free((void **)envp, 0);
	return (exit_code);
}
