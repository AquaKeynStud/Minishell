/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:04:52 by arocca            #+#    #+#             */
/*   Updated: 2025/05/14 22:46:23 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <errno.h>
#include "parsing.h"
#include "minishell.h"
#include "sigaction.h"

static int	count_argv(t_ast *node)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < node->sub_count)
	{
		if (node->childs[i]->type == AST_REDIR)
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	**ast_to_argv(t_ast *node)
{
	int		i;
	int		argc;
	char	**argv;
	int		arg_idx;
	t_ast	**childs;

	if (!node)
		return (NULL);
	i = 0;
	arg_idx = 1;
	childs = node->childs;
	argc = count_argv(node);
	argv = s_malloc((argc + 2) * sizeof(char *));
	argv[0] = node->value;
	argv[argc + 1] = NULL;
	while (i < node->sub_count)
	{
		if (childs[i]->type == AST_REDIR)
			i += 2;
		else
			argv[arg_idx++] = childs[i++]->value;
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
	full_cmd = NULL;
	if (!cmd)
		return (NULL);
	raw_path = get_from_env(env, "PATH");
	if (!raw_path || !*raw_path || ft_strchr(cmd, '/'))
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
		free(args);
	if (envp)
		double_free((void **)envp, 0);
	return (exit_code);
}
