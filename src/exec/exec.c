/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:23:03 by arocca            #+#    #+#             */
/*   Updated: 2025/07/13 10:11:34 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include <errno.h>
#include "parsing.h"
#include <stdbool.h>
#include <sys/wait.h>
#include "minishell.h"
#include "sigaction.h"

static int	exec_side_pipe(t_ctx *ctx, t_ast *node, int fds[2], bool is_l_side)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		sig_set(SIG_DFL);
		if (is_l_side)
			dup2(fds[1], STDOUT_FILENO);
		else
			dup2(fds[0], STDIN_FILENO);
		close_fd(&ctx->fds, fds[0]);
		close_fd(&ctx->fds, fds[1]);
		ctx->status = execute_ast(ctx, node);
		secure_exit(ctx);
	}
	else if (pid < 0)
		perror_code("pipe fork", -1);
	return (pid);
}

static int	exec_pipe(t_ctx *ctx, t_ast *node)
{
	int	ret;
	int	fds[2];
	int	pid_left;
	int	pid_right;

	ret = pipe(fds);
	if (ret != 0)
		return (exit_with_code(ctx, s_exec_exit(ctx->status)));
	register_fd(&ctx->fds, fds[0]);
	register_fd(&ctx->fds, fds[1]);
	sig_set(SIG_IGN);
	pid_left = exec_side_pipe(ctx, node->childs[0], fds, true);
	pid_right = exec_side_pipe(ctx, node->childs[1], fds, false);
	close_fd(&ctx->fds, fds[0]);
	close_fd(&ctx->fds, fds[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &ctx->status, 0);
	sig_init();
	return (s_exec_exit(ctx->status));
}

static int	exec_redir(t_ctx *ctx, t_ast *node)
{
	int	fd;
	int	pid;

	fd = pid_verification(ctx, node);
	if (fd < 0)
		return (ctx->status);
	pid = fork();
	if (pid < 0)
		return (redir_err(ctx, node, 0));
	else if (pid == 0)
	{
		sig_set(SIG_DFL);
		if (node->value[0] == '<')
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		ctx->status = execute_ast(ctx, node->childs[1]);
		secure_exit(ctx);
	}
	close_fd(&ctx->fds, fd);
	waitpid(pid, &ctx->status, 0);
	sig_init();
	return (s_exec_exit(ctx->status));
}

static int	exec_command(t_ctx *ctx, t_ast *node)
{
	int		pid;
	char	*path;
	char	**envp;
	char	**args;

	envp = env_to_envp(ctx, ctx->env);
	args = ast_to_argv(ctx, node);
	path = get_path(ctx, node->value, ctx->env);
	if (!path || !args || !envp)
		return (execve_err(ctx, args));
	sig_set(SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror_code("command fork", 2));
	if (pid == 0)
	{
		sig_set(SIG_DFL);
		close_unregistered_fds(ctx);
		execve(path, args, envp);
		execve_err(ctx, args);
		secure_exit(ctx);
	}
	waitpid(pid, &ctx->status, 0);
	sig_init();
	return (s_exec_exit(ctx->status));
}

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
		if (!in_str(node->value[i], "~$*", false))
			append(ctx, node, &res, &i);
		else if (node->value[i] == '~' && node->quote == NONE)
			i += 1 + expand_tilde(ctx, &res);
		else if (node->value[i] == '$' && node->quote != SINGLE)
			i += 1 + expand_env(ctx, node, &res, (i + 1));
		else
			i++;
		// else if (node->value[i] == '*' && *node->quotes == '0')
		// 	i += 1 + expand_wlcd(ctx, node, res);
	}
	s_free(ctx, node->value);
	node->value = res;
}

int	execute_ast(t_ctx *ctx, t_ast *node)
{
	if (!node)
		return (ctx->status);
	if (node->type == AST_PIPE)
		ctx->status = exec_pipe(ctx, node);
	else if (node->type == AST_REDIR)
		ctx->status = exec_redir(ctx, node);
	else if (node->type == AST_AND || node->type == AST_OR)
		ctx->status = exec_operators(ctx, node);
	else if (node->type == AST_SUB)
		ctx->status = exec_subshell(ctx, node->childs[0]);
	else if (node->type == AST_COMMAND && node->value)
	{
		if (node && node->childs && node->childs[0])
			expand_args(ctx, node->childs[0]);
		if (node->value && !ft_strcmp(node->value, "!"))
			ctx->status = 1;
		else if (node->value && is_builtin(node->value))
			ctx->status = exec_builtin(ctx, ast_to_argv(ctx, node), ctx->env);
		else
			ctx->status = exec_command(ctx, node);
	}
	return (ctx->status);
}
