/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:04:35 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/19 21:54:19 by zpiarova         ###   ########.fr       */
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
# include <sys/wait.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# include "../includes/minishell.h"

typedef enum e_token_type
{
	TOKEN_TEXT, // text must stay at position 0 !!!
	TOKEN_ARG,
	TOKEN_PIPE, // |
	TOKEN_REDIRIN, // <
	TOKEN_REDIROUT, // >
	TOKEN_APPEND, // >>
	TOKEN_HEREDOC, // <<
	TOKEN_FILE,
	TOKEN_PIPE_RD,
	TOKEN_PIPE_WR
}	t_type;

typedef struct s_redir
{
	t_type			type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_redir			*redir;
	struct s_cmd	*next;
}			t_cmd;

typedef struct s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
}					t_token;

typedef struct s_mini
{
	char	**env;
	t_token	*token_list;
	t_cmd	*cmd_list;

}	t_mini;

/* minishell.c */
int	process_input(char *input, t_mini *mini); // should be void, testing
int		show_prompt(t_mini *mini);
void	set_termios(void);

/* builtins.c */
int		cd_builtin(char *path);
int		pwd_builtin(void);
void	exit_builtin(char *status);
char	*env_builtin(char *name);

/* check_input.c */
int		isbq(char *input); // move this later
int 	check_braces_alnum(char *input, int start);
int		check_curly_braces(char *input);
int		check_next_quote(char *input, int i);
int		check_quotes(char *input);
int		check_input(char *input);

/* evaluator.c */
int	evaluator(t_mini *mini);

/* executor.c */
int	executor(t_mini *mini, t_cmd *cmd);

/* exit.c */
int		error_msg(char *msg, t_mini *mini, char *str_1, char *str_2);
void	validator_msg(t_mini *mini, char *object, char *msg);
void	s_error_msg(char *msg);

/* heredoc.c */
char	*heredoc_input(char *delimeter);

/* free.c */
void	free_four_mallocs(char *s1, char *s2, char *s3, char *s4);
void	free_token_list(t_token *token);
void	free_char_pp(char **arr);
void	free_redir(t_redir *redir);
void	free_cmd_list(t_cmd *node);

/* lexer.c */
char	*create_node_value(char *input, int *i); // move later
t_type	get_type(char *value);
int		create_and_add_tok(char *node_value, t_token **token_list, int *hdoc);
char	*process_text(char *text, int *i, int in_sq, int in_dq);
t_token	*get_token_list(char *input);
t_token	*lexer(char *input);

/* lexer_env.c */
char	*handle_word_no_env(char *res, char *text, int *i);
char	*handle_env(char *res, char *text, int *i);
char	*handle_env_in_braces(char *res, char *text, int *i);
char	*handle_env_without_braces(char *res, char *text, int *i);
char	*get_env_value_to_process(char *text);

/* lexer_quotes.c */
int		find_q_or_end(char *text);
int		find_words(char *text);
char	**trim_quotes_in_array(char **head);
char	*str_from_array(char **head);
char	**process_envs_and_quotes(t_token *token);

/* lexer_quotes_utils.c */
int		array_char_len(char **head);
char	*exp_sub(char *str);
int		check_next_char(char c, char c2, int i);

/* parser_heredoc.c */
int		parser_heredoc(t_mini *mini);

/* parser_redir.c */
void	add_back_redir(t_redir **lst, t_redir *new);
t_redir	*create_redir(t_type type, char *value);
t_redir	*find_redirs(t_token *token);

/* parser.c */
void	init_cmd_node(t_cmd *node);
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
int		is_writable_file(const char *path);

/* signal.c */
void	sig_handler(int sig);

/* testing.c */
void	print_token_list(t_mini *mini);
void	print_command_list(t_mini *mini);

/* token_list.c */
t_token	*new_token(char *value, t_type type);
void	add_back_token(t_token **lst, t_token *new);
t_token	*remove_null_tokens(t_token *token);

/* utils.c */
int		iswhitespace(char c);
int		is_alnum(char *str);
char	*process_env(char *name);
int		get_ttokens_len(t_token	*token);

#endif
