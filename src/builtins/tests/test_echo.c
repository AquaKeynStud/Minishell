/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:04:06 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 11:06:37 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[1] && !ft_strcmp(args[1], "-n")) // -n supprime le retour a la ligne a la fin de la chaine de caractere
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

int	main(void)
{
	// Test 1 : echo hello world
	char *test1[] = {"echo", "hello", "world", NULL};
	ft_printf("Test 1: ");
	ft_echo(test1); // output attendu : hello world\n

	// Test 2 : echo -n hello
	char *test2[] = {"echo", "-n", "hello", NULL};
	ft_printf("Test 2: ");
	ft_echo(test2); // output attendu : hello (sans \n)

	// Test 3 : echo sans argument
	char *test3[] = {"echo", NULL};
	ft_printf("Test 3: ");
	ft_echo(test3); // output attendu : \n

	// Test 4 : echo -n
	char *test4[] = {"echo", "-n", NULL};
	ft_printf("Test 4: ");
	ft_echo(test4); // output attendu : (rien, pas de retour à la ligne)

	return 0;
}
