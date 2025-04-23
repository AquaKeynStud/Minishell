/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:15:48 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/23 10:16:16 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	ft_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			ft_printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}
