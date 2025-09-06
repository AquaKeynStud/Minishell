/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:04:52 by arocca            #+#    #+#             */
/*   Updated: 2025/09/06 13:41:11 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include <errno.h>
#include "parsing.h"
#include "minishell.h"
#include "sigaction.h"

static int	count_argv(t_ast *node)
{
	int		i;
	int		count;
	t_ast	*child;

	i = 0;
	count = 0;
	while (i < node->sub_count)
	{
		child = node->childs[i];
		if (child->type == AST_REDIR)
			i += 2;
		else
		{
			if (child->value && (*child->value || child->quote != NONE))
				count++;
			i++;
		}
	}
	if (node->value && (*node->value || node->quote != NONE))
		count++;
	return (count);
}

char	**env_to_envp(t_ctx *ctx, t_env *env)
{
	int		i;
	t_env	*tmp;
	char	**envp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	envp = (char **)s_malloc(ctx, (i + 1) * sizeof(char *));
	if (!fill_envp(ctx, envp, env))
	{
		double_free(ctx, (void **)envp, 0);
		return (NULL);
	}
	return (envp);
}

char	**ast_to_argv(t_ctx *ctx, t_ast *node)
{
	int		i;
	int		argc;
	char	**argv;
	int		arg_idx;
	t_ast	**ast;

	i = 0;
	arg_idx = 0;
	ast = node->childs;
	argc = count_argv(node);
	argv = s_malloc(ctx, (argc + 1) * sizeof(char *));
	argv[arg_idx++] = node->value;
	while (i < node->sub_count)
	{
		if (ast[i]->type == AST_REDIR)
			i += 2;
		else if (ast[i]->value
			&& (*ast[i]->value || ast[i]->quote != NONE))
			argv[arg_idx++] = ast[i++]->value;
		else
			i++;
	}
	argv[arg_idx] = NULL;
	return (argv);
}

char	*get_path(t_ctx *ctx, char *cmd, t_env *env)
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
		return (s_save(ctx, ft_strdup(cmd)));
	paths = ft_split(raw_path, ':');
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_cmd = join_with_delim(ctx, paths[i++], cmd, "/");
		if (!full_cmd || !access(full_cmd, X_OK))
			break ;
		s_free(ctx, full_cmd);
		full_cmd = NULL;
	}
	double_free(ctx, (void **)paths, 0);
	return (full_cmd);
}

void	check_command(t_ctx *ctx, t_ast *ast)
{
	if (ast->value && !ft_strcmp(ast->value, "!"))
		ctx->status = 1;
	else if (ast->value && is_builtin(ast->value))
		ctx->status = exec_builtin(ctx, ast_to_argv(ctx, ast), ctx->env);
	else
		ctx->status = exec_command(ctx, ast);
	return ;
}
