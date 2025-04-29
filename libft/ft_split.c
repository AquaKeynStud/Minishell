/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:57:13 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/25 12:28:17 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_split(char	**tab)
{
	unsigned int	i;

	i = 0;
	if (!tab)
		return (NULL);
	while (tab[i])
		free(tab[i++]);
	free(tab);
	return (NULL);
}

static int	count_words(char const *s, char c)
{
	unsigned int	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
			count += 1;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static char	*split_word(char const *s, char c)
{
	char			*word;
	unsigned int	len;
	unsigned int	i;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = (char *)malloc((sizeof(char) * len) + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char			**res;
	unsigned int	i;

	if (!s)
		return (NULL);
	i = 0;
	res = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!res)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			res[i] = split_word(s, c);
			if (!res[i])
				return (free_split(res));
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	res[i] = NULL;
	return (res);
}
