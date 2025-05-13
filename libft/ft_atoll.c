/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:33:52 by abouclie          #+#    #+#             */
/*   Updated: 2025/05/07 11:34:08 by abouclie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	skip_whitespace_and_sign(const char *str, int *sign)
{
	int	i;

	i = 0;
	*sign = 1;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-')
	{
		*sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	return (i);
}

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

static int	check_overflow(long long res, long long limit, int sign, char digit)
{
	int	max_digit;

	if (sign == 1)
		max_digit = 7;
	else
		max_digit = 8;
	if (res > limit)
		return (2);
	if (res == limit && (digit - '0') > max_digit)
		return (2);
	return (0);
}

long long	ft_atoll(const char *str, int *error)
{
	int			i;
	int			sign;
	long long	res;
	long long	limit;

	i = 0;
	res = 0;
	limit = LLONG_MAX / 10;
	*error = 0;
	i = skip_whitespace_and_sign(str, &sign);
	if (!is_numeric(str + i))
	{
		*error = 2;
		return (0);
	}
	while (str[i] != '\0')
	{
		*error = check_overflow(res, limit, sign, str[i]);
		if (*error == 2)
			return (0);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}
