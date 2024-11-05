# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/24 18:03:05 by vsanin            #+#    #+#              #
#    Updated: 2024/11/05 13:19:05 by zpiarova         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

HEADER = includes/minishell.h

UNAME := $(shell uname)
READLINE_DIR = /usr/local/opt/readline

SRC  =  srcs/minishell.c \
		srcs/builtins.c \
		srcs/env.c \
		srcs/exit.c \
		srcs/lexer.c \
		srcs/paths.c \
		srcs/redirections.c \
		srcs/signal.c \
		srcs/utils.c \

OBJ = $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

ifeq ($(UNAME), Linux)
READLINE = -lreadline

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(READLINE)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@
endif

ifeq ($(UNAME), Darwin)
READLINE = -L$(READLINE_DIR)/lib -I$(READLINE_DIR)/include -lreadline -lncurses

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) -I$(READLINE_DIR)/include -o $(NAME) $(OBJ) $(LIBFT) $(READLINE)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -I$(READLINE_DIR)/include -c $< -o $@
endif

clean:
	$(RM) $(OBJ)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
