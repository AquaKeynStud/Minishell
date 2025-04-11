# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abouclie <abouclie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/20 09:07:20 by abouclie          #+#    #+#              #
#    Updated: 2025/04/10 14:06:54 by abouclie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a
CC		=	cc
FLAGS	=	-Wall -Wextra -Werror -MMD -MP
REMOVE	=	rm -rf
MKDIR	=	mkdir -p

OBJ_DIR	=	obj
DEP_DIR	=	dep

SRCS	=	ft_atoi.c ft_bzero.c ft_calloc.c ft_abs.c\
			ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
			ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
			ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
			ft_split.c ft_strchr.c ft_strndup.c ft_strdup.c ft_striteri.c ft_strjoin.c \
			ft_strlcat.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c \
			ft_strchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
			ft_putnbr.c ft_putstr_fd.c \
			ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c \
			ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c \
			gnl/get_next_line.c gnl/get_next_line_utils.c \
			printf/ft_intlen.c printf/ft_itoa_base.c printf/ft_itoa_ptr.c \
			printf/ft_print_ptr.c printf/ft_printf.c printf/ft_printnbr.c \
			printf/ft_printstr.c printf/ft_ptrlen.c printf/ft_putstr.c

OBJS	=	$(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS	=	$(addprefix $(DEP_DIR)/, $(SRCS:.c=.d))

HEADERS	=	libft.h
MAKEFILE=	Makefile

GREEN	=	\033[0;32m
RESET	=	\033[0m

all:	${NAME}

${NAME}:	${OBJS} ${HEADERS} ${MAKEFILE}
	@ar -rcs $(NAME) $(OBJS)
	@echo "$(GREEN)\n$(NAME): $(NAME) created$(RESET)"

$(OBJ_DIR)/%.o: %.c ${HEADERS} ${MAKEFILE}
	@$(MKDIR) $(dir $@) $(patsubst $(OBJ_DIR)%,$(DEP_DIR)%,$(dir $@))
	@${CC} ${FLAGS} -c $< -o $@ -MF $(patsubst $(OBJ_DIR)%.o,$(DEP_DIR)%.d,$@)
	@echo "$(GREEN)$(NAME): $@ created$(RESET)"

clean:
	@${REMOVE} ${OBJ_DIR} ${DEP_DIR}
	@echo "$(GREEN)\n$(NAME): object files and dependencies deleted$(RESET)"

fclean:		clean
	@${REMOVE} ${NAME}
	@echo "$(GREEN)\n${NAME}: ${NAME} deleted$(RESET)"

re:			fclean all

.PHONY: all clean fclean re

-include $(DEPS)
