/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:13:26 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/14 14:35:41 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "parsing.h"

static int	str_is_numeric(char *arg)
{
	int	i;

	i = 0;
	if (!arg[0])
		return (2);
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (2);
		i++;
	}
	return (0);
}

static void	free_exit(t_ctx *ctx, char **args)
{
	free(args);
	if (ctx->ast)
		free_ast(ctx->ast);
	if (ctx->tokens)
		free_tokens(&ctx->tokens);
	free(ctx->input);
}

static int	check_exit_args(int argc, char **args)
{
	if (argc > 2 && str_is_numeric(args[1]) == 0)
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	if (argc > 1)
	{
		if (str_is_numeric(args[1]) == 2)
		{
			ft_dprintf(2, "minishell: exit: ");
			ft_dprintf(2, "%s: numeric argument required\n", args[1]);
			return (2);
		}
	}
	return (0);
}

void	handle_exit(t_ctx *ctx, int argc, char **args)
{
	long long	arg;

	if (argc > 1)
	{
		arg = ft_atoll(args[1], &ctx->status) % 256;
		if (ctx->status == 2)
		{
			ft_dprintf(2, "minishell: exit: ");
			ft_dprintf(2, "%s: numeric argument required\n", args[1]);
			ctx->status = ctx->status;
			free_exit(ctx, args);
			secure_exit(ctx);
		}
		ctx->status = arg % 256;
		free_exit(ctx, args);
		secure_exit(ctx);
	}
	free_exit(ctx, args);
	secure_exit(ctx);
}

int	ft_exit(t_ctx *ctx, int argc, char **args)
{
	ctx->status = check_exit_args(argc, args);
	if (ctx->status)
	{
		if (ctx->status == 2)
		{
			free_exit(ctx, args);
			secure_exit(ctx);
		}
		return (1);
	}
	handle_exit(ctx, argc, args);
	return (0);
}
