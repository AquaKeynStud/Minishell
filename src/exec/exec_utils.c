/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:04:52 by arocca            #+#    #+#             */
/*   Updated: 2025/04/25 13:59:43 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parsing.h"
#include "minishell.h"

char	**ast_to_argv(t_ast **childs)
{
	char	**argv;
	int		counter;

	counter = 0;
	while (childs[counter])
		counter++;
	argv = s_malloc((counter + 1) * sizeof(char *));
	argv[counter] = NULL;
	while (--counter >= 0)
		argv[counter] = childs[counter]->value;
	return (argv);
}

char	*join_with_delim(const char *s1, const char *s2, const char *delimiter)
{
	char	*tmp;
	char	*final;

	tmp = ft_strjoin(s1, delimiter);
	if (!tmp)
		return (NULL);
	final = ft_strjoin((const char *)tmp, s2);
	free(tmp);
	if (!final)
		perror("malloc");
	return (final);
}

char	*get_path(const char *cmd, t_env *env)
{
	int		i;
	char	**paths;
	char	*raw_path;
	char	*full_cmd;

	i = 0;
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

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "export")
	);
}

int	exec_builtin(char **args, t_env *env)
{
	if (!ft_strcmp(args[0], "cd"))
		return (ft_cd(args, env));
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(args));
	if (!ft_strcmp(args[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(args, env));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(args, env));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env(env));
	if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(args));
	return (-1);
}
