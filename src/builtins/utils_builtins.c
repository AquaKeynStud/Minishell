/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:39:37 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/07 14:17:25 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

char	*get_current_dir(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd");
		return (NULL);
	}
	return (ft_strdup(cwd));
}

int	is_option(char *command, char **args)
{
	int		i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-')
		{
			ft_printf("%s: %s: no option allowed\n", command, args[i]);
			return (2);
		}
		i++;
	}
	return (0);
}