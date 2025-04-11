/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:57:13 by abouclie          #+#    #+#             */
/*   Updated: 2024/11/26 10:46:24 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	reset;

	count = 0;
	reset = 0;
	while (*s)
	{
		if (*s != c && reset == 0)
		{
			reset = 1;
			count++;
		}
		else if (*s == c)
			reset = 0;
		s++;
	}
	return (count);
}

static int	count_char(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s != c && *s != '\0')
	{
		count++;
		s++;
	}
	return (count);
}

static char	*fill_word(const char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = ft_calloc((end - start + 1), sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	return (word);
}

static void	*ft_free(char **str, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		start;
	int		end;

	i = 0;
	start = 0;
	str = ft_calloc(count_words(s, c) + 1, sizeof(char *));
	if (!str)
		return (NULL);
	while (*s && *s == c)
		s++;
	while (i < count_words(s, c))
	{
		end = start + count_char(s + start, c);
		str[i] = fill_word(s, start, end);
		while (s[end] == c)
			end++;
		start = end;
		if (!(str[i]))
			return (ft_free(str, i));
		i++;
	}
	return (str);
}
