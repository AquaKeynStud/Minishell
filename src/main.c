/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:53:51 by arocca            #+#    #+#             */
/*   Updated: 2025/07/23 00:57:01 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include <stdio.h>
#include "lexing.h"
#include <stdlib.h>
#include "parsing.h"
#include "minishell.h"
#include "sigaction.h"
#include <readline/history.h>
#include <readline/readline.h>

static void	init_context(t_ctx *ctx, char **argv, char **envp)
{
	struct stat	st;

	ctx->fds = NULL;
	ctx->ast = NULL;
	ctx->status = 0;
	ctx->input = NULL;
	ctx->tokens = NULL;
	ctx->allocs = NULL;
	ctx->has_found_err = false;
	ctx->err_in_tokens = false;
	ctx->env = init_env(ctx, argv, envp);
	ctx->stdin_fd = dup(STDIN_FILENO);
	ctx->stdout_fd = dup(STDOUT_FILENO);
	if (!stat("/proc/self", &st))
		ctx->uid = s_save(ctx, ft_itoa(st.st_uid));
	return ;
}

static void	destroy_command(t_ctx **ctx, t_token **tokens, t_ast **ast)
{
	free_tokens(*ctx, tokens);
	free_ast(*ctx, *ast);
	close_all_fds(&(*ctx)->fds);
	(*ctx)->ast = NULL;
	(*ctx)->tokens = NULL;
	(*ctx)->has_found_err = false;
	(*ctx)->err_in_tokens = false;
	// if ((*ctx)->status != 0 && ((*ctx)->status - 128) == SIGQUIT)
	// 	ft_dprintf(2, "Quit (core dumped)\n");
	return ;
}

bool	tokens_err(t_ctx *ctx, t_token *tokens)
{
	t_token *tmp;
	int		lpar_counter;

	tmp = tokens;
	lpar_counter = 0;
	if (tmp->type == TOKEN_AND || tmp->type == TOKEN_OR || tmp->type == TOKEN_PIPE || tmp->type == TOKEN_RPAR)
		return (parsing_err(ctx, tmp->value, 2));
	while (tmp->next)
	{
		if (tmp->type == TOKEN_LPAR)
		{
			if (tmp->next->type != TOKEN_WORD && tmp->next->type != TOKEN_LPAR)
				return (parsing_err(ctx, tmp->next->value, 2));
			lpar_counter++;
		}
		else if (tmp->type == TOKEN_RPAR)
		{
			if (lpar_counter <= 0)
				return (parsing_err(ctx, tmp->value, 2));
			if (tmp->next->type == TOKEN_WORD || tmp->next->type == TOKEN_LPAR)
				return (parsing_err(ctx, tmp->next->value, 2));
			lpar_counter--;
		}
		else if ((tmp->type == TOKEN_HEREDOC || tmp->type == TOKEN_REDIR_APPEND || tmp->type == TOKEN_REDIR_IN || tmp->type == TOKEN_REDIR_OUT) && tmp->next->type != TOKEN_WORD)
			return (parsing_err(ctx, tmp->next->value, 2));
		else if ((tmp->type == TOKEN_PIPE || tmp->type == TOKEN_AND || tmp->type == TOKEN_OR) && (tmp->next->type == TOKEN_PIPE || tmp->next->type == TOKEN_AND || tmp->next->type == TOKEN_OR || tmp->next->type == TOKEN_RPAR))
			return (parsing_err(ctx, tmp->next->value, 2));
		tmp = tmp->next;
	}
	if (tmp->type != TOKEN_WORD && tmp->type != TOKEN_RPAR)
		return (parsing_err(ctx, "newline", 2));
	if (tmp->type == TOKEN_RPAR && (lpar_counter--) <= 0)
		return (parsing_err(ctx, ")", 2));
	if (lpar_counter > 0)
		return (parsing_err(ctx, "(", 2));
	return (true);
}

static void	command_handler(t_ctx *ctx, char *cmd)
{
	t_ast	*ast;
	t_token	*tokens;

	tokens = tokenize(ctx, cmd);
	if (!tokens || !tokens_err(ctx, tokens))
	{
		close_unregistered_fds(ctx);
		return (free_tokens(ctx, &tokens));
	}
	ast = parse_input(ctx, tokens);
	ctx->ast = ast;
	ctx->tokens = tokens;
	sig_set(SIG_DFL);
	if (!check_heredoc(ctx, ast))
		return (destroy_command(&ctx, &tokens, &ast));
	close_unregistered_fds(ctx);
	sig_init();
	execute_ast(ctx, ast);
	destroy_command(&ctx, &tokens, &ast);
}

static void	get_input_loop(t_ctx *ctx)
{
	char	*input;

	input = NULL;
	while (1)
	{
		if (COLOR)
		{
			print_status(ctx);
			input = readline(NULL);
		}
		else
			input = readline("minishell => ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		ft_trim(&input, " \t");
		s_save(ctx, input);
		ctx->input = input;
		command_handler(ctx, input);
		s_free(ctx, input);
		ctx->input = NULL;
	}
	rl_clear_history();
	printf("exit\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	// if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
	// {
	// 	if (isatty(STDERR_FILENO))
	// 		ft_dprintf(2, "minishell: interactive mode not allowed\n");
	// 	exit(EXIT_FAILURE);
	// }
	init_context(&ctx, argv, envp);
	set_status(&ctx, 0);
	sig_init();
	get_input_loop(&ctx);
	secure_exit(&ctx);
	return (0);
}
