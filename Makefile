.PHONY : all clean fclean re norminette valgrind exemple

NAME = minishell

# ╭━━━━━━━━━━━━══════════╕出 ❖ BASICS VARIABLES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

CC				:=	cc

CFLAGS			:= -Wall -Wextra -Werror -MMD

RM				:=	rm	-rf

SHOW_MSG_CLEAN	=	true

MAKEFLAGS		+=	--no-print-directory

LIBS			:=	-lreadline -lncurses

INCS			:=	-Iinc/

FILE			=	$(file)

# ╰━━━━━━━━━━━━━━━━════════════════╛出 ❖ 力╘════════════════━━━━━━━━━━━━━━━━╯ #

# ╭━━━━━━━━━━━━══════════╕出 ❖ FILE TREE ❖ 力╒═══════════━━━━━━━━━━━━╮ #

# directories
D_SRC	=	src/
D_INC	=	inc/
D_OBJ	=	.obj/
D_DEP	=	.dep/
D_DOC	=	docs/ # A ENLEVER AVANT DE RENDRE
D_SIG	=	$(D_SRC)signal_handlers/

D_SRCS	= $(D_SRC) $(D_SIG)

# file lists
LST_SRC		=	main.c				\
				readline.c

LST_SIG		=	sig_setup.c			\
				sig_handlers.c

LST_INC		=	readline.h			\
				sigaction.h

LST_SRCS	=	$(LST_SRC) $(LST_SIG)

INC			=	$(addprefix $(D_INC), $(LST_INC))

OBJ			=	$(addprefix $(D_OBJ), $(notdir $(LST_SRCS:.c=.o)))

DEPS		=	$(addprefix $(D_DEPS), $(notdir $(LST_SRCS:.c=.d)))

# ╭━━━━━━━━━━━━══════════╕出 ❖ RULES ❖ 力╒═══════════━━━━━━━━━━━━╮ #

all:	$(NAME)

$(NAME):	$(OBJ) $(INC)
	@$(CC) $(CFLAGS) $(OBJ) $(INCS) $(LIBS) -o minishell
	@echo "\e[0;32mProgramme créé avec succès ! 🧬\e[0m"

$(D_OBJ):
	@mkdir -p $@

$(D_DEP):
	@mkdir -p $(D_DEP)

vpath %.c $(D_SRCS)

$(D_OBJ)%.o: %.c | $(D_OBJ) $(D_DEP)
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@mv $(@:.o=.d) $(D_DEP)

-include $(DEPS)

clean:
ifeq ($(SHOW_MSG_CLEAN), true)
	@echo "\e[0;36mJ'ai enlevé tous les objets relatifs à $(NAME) 🧹\e[0m"
endif
	@$(RM) $(D_OBJ)
	@$(RM) $(D_DEP)
	@$(RM) $(D_DOC)

fclean:
	@$(MAKE) -s SHOW_MSG_CLEAN=false clean
	@$(RM) $(NAME)
	@echo "\e[0;34mExecutable de $(NAME) nettoyé 🧼\e[0m"

re:
	@$(MAKE) fclean
	@$(MAKE) all
	@echo "\e[0;32mProgramme $(NAME) recréé avec succès ! 🫡\e[0m"

exemple: # A ENLEVER AVANT DE RENDRE
	$(CC) $(CFLAGS) $(FILE) $(INCS) $(LIBS) -o $(FILE:.c=)

documentation:
	doxygen Doxyfile

html:
	$(MAKE) documentation
	xdg-open docs/html/index.html 

norminette:
	norminette $(D_SRC) $(D_INC)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

clear: # A ENLEVER AVANT DE RENDRE
	@$(RM) exemples/1_readline exemples/2_history exemples/3_autres_rl