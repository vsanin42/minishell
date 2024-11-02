/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:04:35 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/02 22:10:30 by zuzanapiaro      ###   ########.fr       */
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
# include <limits.h> // PATH_MAX
# include <sys/stat.h> // S_constants
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

#include "../includes/minishell.h"

typedef enum	e_token_type
{
	TOKEN_TEXT, // maybe in parser we separate text types into files, paths, commands/executables, ...
	TOKEN_ARG, // idk if will be used - then we should create array of arguments as it is input for execve
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

/* builtins.c */
int	cd_builtin(char *path);
int	pwd_builtin();
int	unlink_builtin(char *path);

/* exit.c */
int		error_msg(char *msg);

/* lexer.c */
int		check_token(char *str, int *i, t_list **token_list);
t_list	*get_token_list(char *input);
t_list	*lexer(char *input);

/* paths.c */
char	*get_current_directory(void);
char	*get_path_env(char *cmd);
int		is_directory(const char *path);
int		is_regular_file(const char *path);
// char	*get_path_relative(char *str);
// char	*get_path_absolute(char *str);

/* signal.c */
void	sig_handler(int sig);

/* utils */
int		iswhitespace(char c);

#endif
