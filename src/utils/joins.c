/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joins.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:31:20 by arocca            #+#    #+#             */
/*   Updated: 2025/05/13 18:40:34 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*final;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (s2);
	else if (!s2)
		return (s1);
	else if (!*s2)
	{
		free(s2);
		return (s1);
	}
	else if (!*s1)
	{
		free(s1);
		return (s2);
	}
	final = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	if (final)
		return (final);
	return (NULL);
}

char	*join_with_delim(const char *s1, const char *s2, const char *delimiter)
{
	char	*tmp;
	char	*final;

	tmp = ft_strjoin(s1, delimiter);
	if (!tmp)
		return (NULL);
	final = ft_strjoin((const char *)tmp, s2);
	free(tmp);
	if (!final)
		perror("malloc");
	return (final);
}

char	*append_char(char *res, char c)
{
	char	*tmp;
	char	*final;

	tmp = malloc(2);
	if (!tmp)
		return (NULL);
	tmp[0] = c;
	tmp[1] = '\0';
	final = ft_strjoin(res, tmp);
	free(tmp);
	if (!final)
		return (res);
	free(res);
	return (final);
}
