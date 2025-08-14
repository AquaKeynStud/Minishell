.PHONY : all clean fclean re libft norminette valgrind supp_file

.DELETE_ON_ERROR:

NAME = minishell

# ╭━━━━━━━━━━━━══════════╕出 ❖ BASICS VARIABLES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

CC				:=	cc

CFLAGS			:= -Wall -Wextra -Werror -MMD

RM				:=	rm	-rf

SHOW_MSG_CLEAN	=	true

MAKEFLAGS		+=	--no-print-directory

# ╰━━━━━━━━━━━━━━━━════════════════╛出 ❖ 力╘════════════════━━━━━━━━━━━━━━━━╯ #

# ╭━━━━━━━━━━━━══════════╕出 ❖ FILE TREE ❖ 力╒═══════════━━━━━━━━━━━━╮ #

# directories
D_SRC	=	src/
D_INC	=	inc/
D_OBJ	=	.obj/
D_DEP	=	.dep/
D_LFT	=	libft/

D_EXE	=	$(D_SRC)exec/
D_UTL	=	$(D_SRC)utils/
D_TOK	=	$(D_SRC)token/
D_EXP	=	$(D_SRC)expand/
D_PAR	=	$(D_SRC)parsing/
D_BLT	=	$(D_SRC)builtins/
D_SIG	=	$(D_SRC)signal_handlers/

D_SRCS	= $(D_SRC) $(D_EXE) $(D_TOK) $(D_EXP) $(D_BLT) $(D_PAR) $(D_SIG) $(D_UTL)

# file lists
LST_SRC		=	main.c

LST_SIG		=	sig_setup.c			\
				sig_handlers.c

LST_TOK		=	add_token.c			\
				utils_token.c		\
				tokenisation.c		\
				lexing_handlers.c

LST_PAR		=	redir.c				\
				parser.c			\
				parsing_bonus.c		\
				ast_builders.c

LST_EXP		=	merge.c				\
				expand.c			\
				globbing.c

LST_EXE		=	env.c				\
				exec.c				\
				exec_err.c			\
				cmd_utils.c			\
				exec_bonus.c		\
				builtin_exec.c		\
				exec_behaviors.c

LST_BLT		=	ft_cd.c				\
				ft_env.c			\
				ft_pwd.c			\
				ft_exit.c			\
				ft_echo.c			\
				ft_unset.c			\
				ft_export.c			\
				double_dot.c		\
				ft_export_print.c	\
				ft_export_utils.c

LST_UTL		=	str_handle.c		\
				secure_exit.c		\
				secure_alloc.c		\
				fd_collector.c		\
				allocs_collector.c

LST_INC		=	lexing.h			\
				parsing.h			\
				sigaction.h			\
				minishell.h

LST_SRCS	=	$(LST_SRC) $(LST_EXE) $(LST_TOK) $(LST_EXP) $(LST_BLT) $(LST_PAR) $(LST_SIG) $(LST_UTL)

INC			=	$(addprefix $(D_INC), $(LST_INC))

OBJ			=	$(addprefix $(D_OBJ), $(notdir $(LST_SRCS:.c=.o)))

DEPS		=	$(addprefix $(D_DEPS), $(notdir $(LST_SRCS:.c=.d)))

LIBS		:=	-L$(D_LFT) -lft -lreadline -lncurses

INCS		:=	-I$(D_INC) -I$(D_LFT)

LIBFT		:=	$(D_LFT)libft.a

SUPP_FILE	:=	readline.supp

COLOR ?= false

# ╭━━━━━━━━━━━━══════════╕出 ❖ RULES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

all:	$(NAME)

$(NAME): $(LIBFT) $(OBJ) | $(D_OBJ) $(D_DEP) Makefile
	@echo "\033[35mCompilation de $(NAME)...\033[0m"
#	$(info ⏱️  Rebuild check: $?)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
	@echo "\e[0;32m$(NAME) program created successfully ! 🧬\e[0m"

true:
	@$(MAKE) COLOR=true all

$(D_OBJ):
	@mkdir -p $@

$(D_DEP):
	@mkdir -p $@

vpath %.c $(D_SRCS)

$(D_OBJ)%.o: %.c | $(D_OBJ) $(D_DEP)
	@$(CC) $(CFLAGS) -g3 -D COLOR=$(COLOR) $(INCS) -c $< -o $@ -MF $(D_DEP)$(notdir $*.d)
	@echo "\033[34m$(NAME): $@ created\033[0m"

-include $(DEPS)

$(LIBFT):
	$(MAKE) -C $(D_LFT)

clean:
ifeq ($(SHOW_MSG_CLEAN), true)
	@echo "\e[0;36mAll $(NAME) objects have been removed 🧹\e[0m"
endif
	@$(MAKE) -s -C $(D_LFT) clean
	@$(RM) $(D_OBJ) $(D_DEP)

fclean:
	@$(MAKE) -s SHOW_MSG_CLEAN=false clean
	@$(MAKE) -s -C $(D_LFT) fclean
	@$(RM) $(NAME)
	@echo "\e[0;34m$(NAME) executable deleted ! 🧼\e[0m"

re:
	@$(MAKE) fclean
	@$(MAKE) all
	@echo "\e[0;32m$(NAME) program recreated successfully ! 🫡\e[0m"

norminette:
	norminette $(D_SRC) $(D_LFT) $(D_INC)

supp_file: | $(D_OBJ)
	@echo "Generating $(SUPP_FILE)..."
	@printf '{\n	ignore_libreadline_leaks\n	Memcheck:Leak\n	...\n	obj:*/libreadline.so*\n}\n' > $(D_OBJ)$(SUPP_FILE)
	@echo "$(SUPP_FILE) successfully created !"

valgrind: supp_file
	@$(MAKE)
	@clear
	valgrind							\
		--leak-check=full					\
		--show-leak-kinds=all				\
		--track-origins=yes 				\
		--track-fds=yes						\
		--suppressions=$(D_OBJ)$(SUPP_FILE)	\
		./$(NAME)

childs: supp_file
	@$(MAKE)
	@clear
	valgrind							\
		--leak-check=full					\
		--show-leak-kinds=all				\
		--track-origins=yes 				\
		--track-fds=yes						\
		--trace-children=yes				\
		--suppressions=$(D_OBJ)$(SUPP_FILE)	\
		./$(NAME)