.PHONY : all clean fclean re norminette valgrind libft

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
D_DOC	=	docs/ # A ENLEVER AVANT DE RENDRE
D_OBJ	=	.obj/
D_DEP	=	.dep/
D_LFT	=	libft/
D_TOK	=	$(D_SRC)token/
D_PAR	=	$(D_SRC)parsing/
D_SIG	=	$(D_SRC)signal_handlers/

D_SRCS	= $(D_SRC) $(D_SIG) $(D_TOK) $(D_PAR)

# file lists
LST_SRC		=	main.c				\
				readline.c

LST_SIG		=	sig_setup.c			\
				sig_handlers.c

LST_TOK		=	add_token.c			\
				utils_token.c		\
				tokenisation.c

LST_PAR		=	ast_adders.c		\
				ast_setters.c

# LST_UTL

LST_INC		=	utils.h				\
				lexing.h			\
				parsing.h			\
				readline.h			\
				sigaction.h

LST_SRCS	=	$(LST_SRC) $(LST_SIG) $(LST_TOK) $(LST_PAR) #$(LST_UTL)

INC			=	$(addprefix $(D_INC), $(LST_INC))

OBJ			=	$(addprefix $(D_OBJ), $(notdir $(LST_SRCS:.c=.o)))

DEPS		=	$(addprefix $(D_DEPS), $(notdir $(LST_SRCS:.c=.d)))

LIBS			:=	-L$(D_LFT) -lft -lreadline -lncurses

INCS			:=	-I$(D_INC) -I$(D_LFT)

# ╭━━━━━━━━━━━━══════════╕出 ❖ RULES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

all:	$(NAME)

$(NAME):	libft $(OBJ) $(INC) | $(D_OBJ) $(D_DEP) Makefile
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o minishell
	@echo "\e[0;32m$(NAME) program created successfully ! 🧬\e[0m"

$(D_OBJ):
	@mkdir -p $@

$(D_DEP):
	@mkdir -p $(D_DEP)

vpath %.c $(D_SRCS)

$(D_OBJ)%.o: %.c | $(D_OBJ) $(D_DEP)
# @echo "Compiling $< → $@"
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@mv $(@:.o=.d) $(D_DEP)

-include $(DEPS)

libft:	$(D_LFT)
	$(MAKE) -C $(D_LFT)

clean:
ifeq ($(SHOW_MSG_CLEAN), true)
	@echo "\e[0;36mAll $(NAME) objects have been removed 🧹\e[0m"
endif
	@$(MAKE) -s -C $(D_LFT) clean
	@$(RM) $(D_OBJ) $(D_DEP) $(D_DOC)

fclean:
	@$(MAKE) -s SHOW_MSG_CLEAN=false clean
	@$(MAKE) -s -C $(D_LFT) fclean
	@$(RM) $(NAME)
	@echo "\e[0;34m$(NAME) executable deleted ! 🧼\e[0m"

re:
	@$(MAKE) fclean
	@$(MAKE) all
	@echo "\e[0;32m$(NAME) program recreated successfully ! 🫡\e[0m"

documentation:
	doxygen Doxyfile

html:
	$(MAKE) documentation
	xdg-open docs/html/index.html 

norminette:
	norminette $(D_SRC) $(D_INC)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./$(NAME)