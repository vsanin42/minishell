/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:04:35 by vsanin            #+#    #+#             */
/*   Updated: 2024/10/31 16:16:50 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE
# define ERROR 1
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <termios.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

#include "../includes/minishell.h"

typedef enum	e_token_type
{
	TOKEN_TEXT,
	TOKEN_ARG, // idk if will be used
	TOKEN_PIPE,
	TOKEN_SQUOTE, // '
	TOKEN_DQUOTE, // "
	TOKEN_REDIRIN, // <
	TOKEN_REDIROUT, // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_HEREDOC // <<
}	t_token_type;

typedef struct	s_token
{
	char			*value;
	t_token_type	type;
}	t_token;


/* minishell.c */
t_list	*process_input(char *input); // should be void, testing
int		show_prompt(void);
void	set_termios(void);

/* exit.c */
int		error_msg(char *msg);

/* lexer.c */
int		check_token(char *str, int *i, t_list **token_list);
t_list	*get_token_list(char *input);
t_list	*lexer(char *input);

/* signal.c */
void	sig_handler(int sig);

/* utils */
int		iswhitespace(char c);

#endif
