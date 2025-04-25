/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arocca <arocca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:31:01 by arocca            #+#    #+#             */
/*   Updated: 2025/01/15 14:31:01 by arocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		parse_args(char *s, va_list *args, ssize_t *total_len);
int		mandatory_parser(char c, va_list *args, ssize_t *total_len, int err);
int		ft_printf(const char *format, ...)
		__attribute__ ((format (printf, 1, 2)));

char	check_conv(char c);
void	format_update(char **s, ssize_t	*i, ssize_t *len, va_list *args);

int		handle_string(va_list *args, ssize_t *total_len, int err);
int		handle_char(va_list *args, ssize_t *total_len, int err);
int		handle_nbr(va_list *args, ssize_t *total_len, int isInt, int err);
int		handle_address(va_list *args, ssize_t *total_len, int err);
int		handle_hexa(va_list *args, ssize_t *total_len, int isLower, int err);

void	ft_print_memory(void *addr, ssize_t *total_len);
void	convert_to(unsigned long a, char *base, ssize_t *total_len);
void	ft_putnbr_pt(int n, int fd, ssize_t *total_len, int (*f)[8]);
void	ft_putunbr_fd(unsigned int n, int fd, ssize_t *total_len, int (*f)[8]);

int		c_d(int nb);
int		authorized_c(char c);
int		addr_len(void *addr);
int		x_len(unsigned int nb);
int		fill(char c, int len, int (*f)[8], int both);

void	create_addr(va_list *args, ssize_t *total_len, int (*f)[8]);
void	create_string(va_list *args, ssize_t *total_len, int (*f)[8]);
void	create_x(char c, va_list *args, ssize_t *total_len, int (*f)[8]);
void	create_nbr(char c, va_list *args, ssize_t *total_len, int (*f)[8]);

void	write_sign(int *n, ssize_t *total_len, int (*f)[8]);
void	print_nb_bonus(int nbr, ssize_t *total_len, int (*f)[8]);
void	print_char_bonus(int n, ssize_t *total_len, int (*f)[8]);
void	print_str_bonus(char *str, ssize_t *total_len, int (*f)[8]);
void	print_addr_bonus(void *arg, ssize_t *total_len, int (*f)[8]);
void	ft_print_memory_bonus(void *addr, ssize_t *total_len, int (*f)[8]);
void	print_uint_bonus(unsigned int arg, ssize_t *total_len, int (*f)[8]);
void	print_x_left(unsigned int n, ssize_t *total_len, int (*f)[8], char **b);
void	print_hexa_b(unsigned int n, ssize_t *total_len, int (*f)[8], char **b);

int		error_tab(char *s);
int		error_parser(char *s, size_t len, ssize_t *total_len);
int		handle_tab_two_char_err(char *s, size_t len, ssize_t *total_len);
int		handle_char_err(char *s, size_t len, ssize_t *total_len, va_list *args);

#endif