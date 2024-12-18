# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/18 15:06:10 by vsanin            #+#    #+#              #
#    Updated: 2024/12/18 15:06:13 by vsanin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
# nm -u minishell: use in shell to check for functions not linked into executable thus not ours
RM = rm -f

LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

HEADER = includes/minishell.h

UNAME := $(shell uname)
READLINE_DIR = /usr/local/opt/readline

SRC  =  srcs/minishell.c \
		srcs/builtins/builtins.c \
		srcs/builtins/cd.c \
		srcs/builtins/echo.c \
		srcs/builtins/env.c \
		srcs/builtins/exit.c \
		srcs/builtins/export.c \
		srcs/builtins/pwd.c \
		srcs/builtins/unset.c \
		srcs/envs/env_utils_2.c \
		srcs/envs/env_utils.c \
		srcs/envs/expanding_env.c \
		srcs/envs/expanding_utils.c \
		srcs/evaluators/cmd_evaluator.c \
		srcs/evaluators/input_evaluator.c \
		srcs/evaluators/token_evaluator.c \
		srcs/execution/executor.c \
		srcs/execution/ex_utils_2.c \
		srcs/execution/ex_utils.c \
		srcs/execution/ex_files_pipes.c \
		srcs/execution/ex_stdin_stdout.c \
		srcs/lexer/lexer_quotes_utils.c \
		srcs/lexer/lexer_quotes.c \
		srcs/lexer/lexer_utils.c \
		srcs/lexer/lexer.c \
		srcs/parser/heredoc_utils.c \
		srcs/parser/parser_heredoc.c \
		srcs/parser/parser.c \
		srcs/types/array.c \
		srcs/types/t_cmd.c \
		srcs/types/t_cmd_utils.c \
		srcs/types/t_redir.c \
		srcs/types/t_token.c \
		srcs/types/t_token_utils.c \
		srcs/utils/messages_2.c \
		srcs/utils/messages.c \
		srcs/utils/file_utils.c \
		srcs/utils/free.c \
		srcs/utils/signal.c \
		srcs/utils/testing.c \
		srcs/utils/utils.c \

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
