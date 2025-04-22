/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:38:19 by arocca            #+#    #+#             */
/*   Updated: 2025/04/22 17:04:40 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "minishell.h"

int execute_pipe(t_ast *node, t_ctx *ctx)
{
    int    ret;
    int    pid;
    int    status;
    int    fds[2];

    ret = pipe(fds);
    if (ret != 0)
        return (1);
    open_fd(&ctx->fds, fds[0]);
    open_fd(&ctx->fds, fds[1]);
    pid = fork();
    if (pid == 0)
    {
        dup2(fds[1], STDOUT_FILENO);
        close_fd(&ctx->fds, fds[0]);
        close_fd(&ctx->fds, fds[1]);
        execute_ast(node->childs[0], ctx);
        exit(ctx->status);
    }
    pid = fork();
    if (pid == 0)
    {
        dup2(fds[0], STDIN_FILENO);
        close_fd(&ctx->fds, fds[0]);
        close_fd(&ctx->fds, fds[1]);
        execute_ast(node->childs[1], ctx);
        exit(ctx->status);
    }
    close_fd(&ctx->fds, fds[0]);
    close_fd(&ctx->fds, fds[1]);
    waitpid(-1, &status, 0);
    waitpid(-1, &status, 0);
    return (WEXITSTATUS(status));
}

int execute_redirection(t_ast *node, t_ctx *ctx)
{
    int    fd;
    int    status;

    if (strcmp(node->value, ">") == 0)
        fd = open(node->childs[0]->value, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    else if (strcmp(node->value, ">>") == 0)
        fd = open(node->childs[0]->value, O_CREAT | O_APPEND | O_WRONLY, 0644);
    else if (strcmp(node->value, "<") == 0)
        fd = open(node->childs[0]->value, O_RDONLY);
    else /* "<<" */
        fd = here_doc(node->childs[0]->value);
    if (fd < 0)
		perr("redir", 1);
    if (node->value[0] == '<')
        dup2(fd, STDIN_FILENO);
    else
        dup2(fd, STDOUT_FILENO);
    open_fd(&ctx->fds, fd);
    status = execute_ast(node->childs[1], ctx);
    dup2(ctx->stdin_fd, STDIN_FILENO);
    dup2(ctx->stdout_fd, STDOUT_FILENO);
    close_fd(&ctx->fds, fd);
    return (status);
}

int execute_command(t_ast *node, t_ctx *ctx)
{
    int		pid;
    int		wstatus;
    char 	**args;

    if (is_builtin(node->value))
        return (run_builtin(node, ctx));
    pid = fork();
    if (pid == 0)
    {
        args = node->childs;
        execve(resolve_path(node->value, ctx->env),
               args, list_to_envp(ctx->env));
        perr("execve", 1);
    }
    waitpid(pid, &wstatus, 0);
    return (WEXITSTATUS(wstatus));
}

int execute_ast(t_ctx *ctx, t_ast *node)
{
	if (!node) 
		return (ctx->status);
	if (node->type == AST_PIPE)
		ctx->status = execute_pipe(node, ctx);
	else if (node->type == AST_REDIR)
		ctx->status = execute_redirection(node, ctx);
	else
		ctx->status = execute_command(node, ctx);
	return (ctx->status);
}
