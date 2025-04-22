/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:26:39 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/22 12:28:32 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

int	ft_unset(char **args, t_env *env)
{
	int		i;
	t_env	*prev;
	t_env	*tmp;

	i = 1;
	while (args[i])
	{
		prev = NULL;
		tmp = env;
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, args[i]))
			{
				if (prev)
					prev->next = tmp->next;
				else
					env = tmp->next;
				free(tmp->key);
				free(tmp->value);
				free(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (0);
}
