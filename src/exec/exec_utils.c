/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:04:52 by arocca            #+#    #+#             */
/*   Updated: 2025/05/13 18:33:49 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parsing.h"
#include "minishell.h"
#include "sigaction.h"
#include <readline/history.h>
#include <readline/readline.h>

char	**ast_to_argv(t_ast *node)
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
	argv[0] = node->value;
	while (pos > 0)
	{
		argv[pos] = childs[pos - 1]->value;
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
		free(args);
	if (envp)
		double_free((void **)envp, 0);
	return (exit_code);
}
