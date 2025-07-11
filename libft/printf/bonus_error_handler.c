/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_error_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 11:53:39 by keyn              #+#    #+#             */
/*   Updated: 2025/04/27 12:49:08 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_char_err(char *s, size_t len, ssize_t *total_len, va_list *args)
{
	if (mandatory_parser(s[len + 1], args, total_len, 1))
		return (mandatory_parser(s[len + 1], args, total_len, 1) + 2);
	else
		return ((int)(*total_len -= (*total_len + 1)));
}

int	handle_tab_two_char_err(char *s, size_t len, ssize_t *total_len)
{
	if (!check_conv(s[0]) && !check_conv(s[1])
		&& auth_char(s[0]) == -1 && s[1])
		return (*total_len += write(1, "%", 1));
	if (error_tab(s) || (auth_char(s[0]) == -1 && !check_conv(s[0])
			&& !check_conv(s[1])))
	{
		*total_len += write(1, "%", 1);
		len += error_parser(s, len, total_len);
		return (len);
	}
	return (-1);
}

int	error_parser(char *s, size_t len, ssize_t *total_len)
{
	while (auth_char(s[len]) >= 0)
	{
		if (!(len == 0 && s[len] == '0'))
			*total_len += write(1, &s[len], 1);
		if (s[len] == '.' && s[len + 1] == ' ')
			*total_len += write(1, "0", 1);
		len++;
	}
	return (len);
}
