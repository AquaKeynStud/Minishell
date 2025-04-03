# MAKEFILE TEMPORAIRE

.PHONY: all valgrind clean exemple

CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror
LIBS	:= -lreadline -lncurses
INCS	:= -Iinc/

FILE = $(file)

all:
	$(CC) $(CFLAGS) src/main.c $(INCS) $(LIBS) -o minishell

exemple:
	$(CC) $(CFLAGS) $(FILE) $(INCS) $(LIBS) -o $(FILE:.c=)

clean:
	rm -f minishell exemples/1_readline exemples/2_history exemples/3_autres_rl