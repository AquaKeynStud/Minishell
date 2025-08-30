.PHONY : all clean fclean re

.DELETE_ON_ERROR:

NAME := libft.a

# ╭━━━━━━━━━━━━══════════╕出 ❖ BASICS VARIABLES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

CC				:=	cc

CFLAGS			:=	-Wall -Wextra -Werror -MMD -MP

RM				:=	rm	-rf

SHOW_MSG_CLEAN	=	true

MAKEFLAGS		+=	--no-print-directory

# ╰━━━━━━━━━━━━━━━━════════════════╛出 ❖ 力╘════════════════━━━━━━━━━━━━━━━━╯ #

# ╭━━━━━━━━━━━━══════════╕出 ❖ FILE TREE ❖ 力╒═══════════━━━━━━━━━━━━╮ #

# directories
D_OBJ	=	.obj/
D_DEP	=	.dep/
D_PTF	=	./printf/
D_GNL	=	./gnl/

LST_SRC	=	ft_abs.c				\
			in_str.c				\
			ft_atoi.c				\
			ft_itoa.c				\
			ft_trim.c				\
			ft_atoll.c				\
			ft_bzero.c				\
			ft_split.c				\
			ft_calloc.c				\
			ft_memchr.c				\
			ft_memcmp.c				\
			ft_memcpy.c				\
			ft_memset.c				\
			ft_strchr.c				\
			ft_strcmp.c				\
			ft_lstmap.c				\
			ft_lstnew.c				\
			ft_strdup.c				\
			ft_strlen.c				\
			ft_substr.c				\
			ft_putnbr.c				\
			ft_isalnum.c			\
			ft_isalpha.c			\
			ft_isascii.c			\
			ft_isdigit.c			\
			ft_isprint.c			\
			ft_memmove.c			\
			ft_realloc.c			\
			ft_strndup.c			\
			ft_strjoin.c			\
			ft_strlcat.c			\
			ft_strlcpy.c			\
			ft_strmapi.c			\
			ft_strncmp.c			\
			ft_strncpy.c			\
			ft_strnstr.c			\
			ft_strrchr.c			\
			ft_strtrim.c			\
			ft_lstiter.c			\
			ft_lstlast.c			\
			ft_lstsize.c			\
			ft_tolower.c			\
			ft_toupper.c			\
			ft_striteri.c			\
			ft_lstclear.c			\
			ft_split_str.c			\
			ft_putnbr_fd.c			\
			ft_putstr_fd.c			\
			ft_lstdelone.c			\
			ft_putchar_fd.c			\
			ft_putendl_fd.c			\
			is_whitespace.c			\
			ft_lstadd_back.c		\
			ft_lstadd_front.c		\
			gnl/get_next_line.c		\
			gnl/get_next_line_utils.c

LST_PTF	=	main.c					\
			utils.c					\
			handlers.c				\
			printers.c				\
			bonus_utils.c			\
			bonus_parser.c			\
			bonus_setter.c			\
			dprintf_main.c			\
			dprintf_handlers.c		\
			bonus_printers_diuc.c	\
			bonus_printers_spxX.c	\
			bonus_error_handler.c	\

LST_GNL	=	get_next_line.c			\
			get_next_line_utils.c

LST_INC		=	libft.h ft_printf.h

LST_SRCS	=	$(LST_SRC) $(LST_PTF) $(LST_GNL)

OBJ			=	$(addprefix $(D_OBJ), $(notdir $(LST_SRCS:.c=.o)))

INC			:=	-I.

DEP		=	$(addprefix $(D_DEP), $(notdir $(LST_SRCS:.c=.d)))

GREEN	=	\033[0;32m
YELLOW	=	\033[33m
RESET	=	\033[0m

all:	$(NAME)

$(NAME): $(OBJ) $(LST_INC) Makefile | $(D_OBJ) $(D_DEP) Makefile
	@ar -rcs $(NAME) $(OBJ)
	@echo "$(GREEN)\nLIBFT: $(NAME) created$(RESET)\n\n"

$(D_OBJ):
	@mkdir -p $@

$(D_DEP):
	@mkdir -p $@

vpath %.c . $(D_PTF) $(D_GNL)

$(D_OBJ)%.o: %.c $(LST_INC) Makefile | $(D_OBJ) $(D_DEP) Makefile
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@ -MF $(D_DEP)$(notdir $*.d)
	@echo "$(YELLOW)LIBFT: $@ created$(RESET)"

-include $(DEPS)

clean:
	@$(RM) $(D_OBJ) $(D_DEP)
	@echo "$(GREEN)$(NAME): object files and dependencies deleted$(RESET)\n"

fclean:
	@$(MAKE) clean
	@$(RM) $(NAME)
	@echo "$(GREEN)$(NAME): $(NAME) deleted$(RESET)\n"

re:
	@$(MAKE) fclean
	@$(MAKE) all
	@echo "\e[0;32m$(NAME) archive recreated successfully ! 🫡\e[0m"

norminette:
	norminette . $(D_PTF) $(D_GNL)
