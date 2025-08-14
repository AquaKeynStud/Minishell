/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:57:13 by abouclie          #+#    #+#             */
/*   Updated: 2025/04/24 17:39:59 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_sep(char c, char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

static int	count_words(char *str, char *charset)
{
	int	i;

	i = 0;
	while (*str)
	{
		while (*str && is_sep(*str, charset))
			str++;
		if (*str)
			i += 1;
		while (*str && !is_sep(*str, charset))
			str++;
	}
	return (i);
}

static char	*split_word(char *str, char *charset)
{
	char	*word;
	int		i;
	int		j;

	i = 0;
	while (str[i] && !is_sep(str[i], charset))
		i++;
	word = (char *)malloc(sizeof(char) * i + 1);
	if (!word)
		return (0);
	j = 0;
	while (j < i)
	{
		word[j] = str[j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

char	**ft_split_str(char *str, char *charset)
{
	char	**res;
	int		i;

	i = 0;
	if (!str || !*str || !charset)
		return (NULL);
	res = (char **)malloc(sizeof(char *) * (count_words(str, charset) + 1));
	if (!res)
		return (0);
	while (*str)
	{
		while (*str && is_sep(*str, charset))
			str++;
		if (*str)
		{
			res[i] = split_word(str, charset);
			i++;
			while (*str && !is_sep(*str, charset))
				str++;
		}
	}
	res[i] = NULL;
	return (res);
}
