/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:09:36 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 11:10:35 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

int	ft_pwd()
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		ft_printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

int	main(void)
{
	ft_printf("Test ft_pwd:\n");
	ft_pwd(); // Affiche le répertoire courant

	// Tu peux tester aussi un changement de dossier :
	chdir("..");
	ft_printf("Test ft_pwd après chdir(\"..\"):\n");
	ft_pwd();

	return 0;
}