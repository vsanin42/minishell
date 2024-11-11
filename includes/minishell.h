/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:04:35 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/11 15:31:44 by zpiarova         ###   ########.fr       */
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
	/* 0 */ TOKEN_TEXT, // maybe in parser we separate text types into files, paths, commands/executables, ...
	/* 1 */ TOKEN_ARG, // idk if will be used - later we could create array of arguments because it is needed as input for execve
	/* 2 */ TOKEN_PIPE, // |
	/* 3 */ TOKEN_REDIRIN, // <
	/* 4 */ TOKEN_REDIROUT, // >
	/* 5 */ TOKEN_APPEND, // >>
	/* 6 */ TOKEN_HEREDOC, // <<
	/* 7 */TOKEN_FILE
}	t_token_type;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;


typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_redir			*redir;
	// int				heredoc; // 0 default, 1 if followed by <<
	// int				append;
	struct s_cmd	*next;
}			t_cmd;


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
	t_cmd	*cmd_list;

}	t_mini;

/* minishell.c */
t_cmd	*process_input(char *input, t_mini *mini); // should be void, testing
int		show_prompt(t_mini *mini);
void	set_termios(void);

/* builtins.c */
int		cd_builtin(char *path);
int		pwd_builtin();
void	exit_builtin(char *status);
char	*env_builtin(char *name);

/* env_quotes.c */
int		find_q_or_end(char *text);
int		find_words(char *text);
char	**trim_quotes_in_array(char **head);
char	*str_from_array(char **head);
char 	**parse_eq(t_token *token);

/* env_utils_v.c */
int		array_char_len(char **head);
char	*exp_sub(char *str);
int		check_next_char(char c, char c2, int i);

/* env.c */
char	*handle_word_no_env(char *res, char *text, int *i);
char	*handle_env(char *res, char *text, int *i);
char	*handle_env_in_braces(char *res, char *text, int *i);
char	*handle_env_without_braces(char *res, char *text, int *i);
char	*get_env_value_to_process(char *text);

/* exit.c */
int		error_msg(char *msg, t_mini *mini, char *string_to_free_1, char *str_to_free_2);

/* free.c */
void	free_four_mallocs(char *s1, char *s2, char *s3, char *s4);
void	free_token_list(t_token *token);
void	free_char_pp(char **arr);
void	free_redir(t_redir *redir);
void	free_cmd_list(t_cmd *node);

/* lexer.c */
int		check_token(char *input, int i, t_token **token_list);
t_token	*get_token_list(char *input);
t_token	*lexer(char *input);
void	parse_envs_and_quotes(t_token *token);

/* parser_redir.c */
void	add_back_redir(t_redir **lst, t_redir *new);
t_redir	*create_redir(t_token_type type, char *value);
t_redir	*find_redirs(t_token *token);
void	init_cmd_node(t_cmd *node);

/* parser.c */
int		get_ttokens_len(t_token	*token);
char	**alloc_args(char **args, t_token *token);
void	add_back_cmd(t_cmd **lst, t_cmd *new);
t_cmd	*new_cmd(t_token *token);
t_cmd	*parser(t_mini *mini);

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

/* testing.c */
void	print_token_list(t_mini *mini);
void	print_command_list(t_mini *mini);

/* token_list.c */
t_token	*new_token(char *value, t_token_type type);
void	add_back_token(t_token **lst, t_token *new);

/* utils.c */
int		iswhitespace(char c);
int		is_alnum(char *str);
char	*process_env(char *name);

#endif
