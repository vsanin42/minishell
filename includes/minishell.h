/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:04:35 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/04 19:01:42 by vsanin           ###   ########.fr       */
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
# include <sys/types.h> // DIR
# include <dirent.h> // opendir, readdir, closedit
# include <limits.h> // PATH_MAX
# include <sys/stat.h> // S_constants
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

#include "../includes/minishell.h"

typedef enum	e_token_type
{
	TOKEN_TEXT, // maybe in parser we separate text types into files, paths, commands/executables, ...
	TOKEN_ARG, // idk if will be used - later we could create array of arguments because it is needed as input for execve
	TOKEN_PIPE, // |
	TOKEN_SQUOTE, // '
	TOKEN_DQUOTE, // "
	TOKEN_REDIRIN, // <
	TOKEN_REDIROUT, // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_HEREDOC, // <<
	TOKEN_ENV
}	t_token_type;

typedef struct	s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct	s_mini
{
	char	**env;
	t_token	*token_list;
	
}	t_mini;

/* minishell.c */
t_token	*process_input(char *input, t_mini *mini); // should be void, testing
int		show_prompt(t_mini *mini);
void	set_termios(void);

/* builtins.c */
int		cd_builtin(char *path);
int		pwd_builtin();
void	exit_builtin(char *status);
char	*handle_env(char *name);

/* exit.c */
int		error_msg(char *msg);

/* lexer.c */
int		check_token(char *input, int i, t_token **token_list);
t_token	*get_token_list(char *input);
t_token	*lexer(char *input);

/* paths.c */
char	*get_current_directory(void);
char	*get_path_env(char *cmd);
int		is_directory(const char *path);
int		is_executable_file(const char *path);
int		is_readable_file(const char *path);

/* redirections.c */
int	redirect_input(char *file);

/* signal.c */
void	sig_handler(int sig);

/* utils.c */
int		iswhitespace(char c);
t_token	*new_token(char *value, t_token_type type);
void	add_back_token(t_token **lst, t_token *new);
void	clear_token_list(t_token *token);

#endif
