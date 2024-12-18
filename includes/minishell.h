/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:04:35 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/18 12:54:48 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE
# define ERROR 1
# define EXP 1
# define NO_EXP 0
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <termios.h>
# include <sys/types.h>
# include <dirent.h>
# include <limits.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# include "../includes/minishell.h"

extern int	g_sig;

typedef enum e_token_type
{
	TOKEN_TEXT,
	TOKEN_PIPE,
	TOKEN_REDIRIN,
	TOKEN_REDIROUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_FILE
}	t_type;

typedef struct s_redir
{
	t_type			type;
	char			*file;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	t_redir			*redir;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
}					t_token;

typedef struct s_mini
{
	char			**env;
	t_token			*token_list;
	t_cmd			*cmd_list;
	char			*error_msg;
	int				exit_status;
	int				**pipes;
	int				*pids;
}					t_mini;

/* minishell.c */
int		process_input(char *input, t_mini *mini);
int		show_prompt(t_mini *mini);
void	set_termios(int mode);

/* builtins/~.c */
int		is_builtin(t_cmd *cmd);
void	update_pwd_env(char **env);
int		cd_builtin(t_mini *mini, t_cmd *cmd);
char	*get_current_directory(void);
int		pwd_builtin(t_mini *mini, t_cmd *cmd);
int		exit_builtin(t_mini *mini, t_cmd *cmd);
int		export_builtin(t_mini *mini, t_cmd *cmd);
int		env_builtin(t_mini *mini, t_cmd *cmd, char *prefix);
int		unset_builtin(t_mini *mini, t_cmd *cmd);
int		unset_strdup(char **result, char **env, int *i, int flag);
int		echo_builtin(t_mini *mini, t_cmd *cmd);
char	*echo_builder(char **args);
int		echo_n_option(char **args, int i);
int		echo_skip_n(char **tmp);

/* envs/env_utils_2.c */
void	free_paths(char **paths, int *i);
int		has_env_value(char *env);
int		check_env_name(char *env);

/* envs/env_utils.c */
char	*process_local_env(t_mini *mini, char *name);
char	*get_path_env(t_mini *mini, char *cmd);
int		get_env_index_by_name(char **envs, char *env_name);
char	*getenv_local(char **envs, char *env_name);
char	*extract_env_name(char *env);

/* envs/expanding_env.c */
char	*handle_word_no_env(char *res, char *text, int *i);
char	*handle_env(t_mini *mini, char *res, char *text, int *i);
char	*handle_env_in_braces(t_mini *mini, char *res, char *text, int *i);
char	*handle_env_without_braces(t_mini *mini, char *res, char *text, int *i);
char	*get_env_value_to_process(t_mini *mini, char *text);

/* envs/expanding_utils.c */
char	*handle_question(t_mini *mini, char *res, char *text, int *i);
char	*quickjoin(char *res);

/* evaluators/input-evaluator.c - top 3 not used*/
int		check_braces_alnum(char *input, int start);
int		check_next_quote(char *input, int i);
int		isbq(char *input);

int		check_curly_braces(char *input);
int		check_quotes(char *input);
int		check_bad_substitution(char *input, int i);
int		check_bs_base(char *input);
int		check_input(char *input, t_mini *mini);

/* evaluators/cmd_evaluator.c */
int		validate_files(t_mini *mini, t_redir *red, char *err);
int		cmd_evaluator(t_mini *mini, t_cmd *cmd);

/* evaluators/token_evaluator.c */
int		token_evaluator(t_mini *mini);

/* execution/executor.c */
void	execute(t_mini *mini, t_cmd *cmd);
int		executor(t_mini *mini, int num_of_p);
void	exec_cmd(t_mini *mini, int **pipes, int files[], int i);
int		exec_builtin_in_parent(t_mini *mini, int files[2]);
void	free_int_arr(int **pipes, int *pids);

/* execution/ex_utils.c */
int		get_exit_status(int num_of_p, t_mini *mini, int *pids);
int		exec_builtins(t_mini *mini, t_cmd *cmd);
int		exec_command_by_path(t_mini *mini, t_cmd *cmd);
int		exec_shell_command(t_mini *mini, t_cmd *cmd);
int		init_int_arrs(t_mini *mini, int num_of_p);

/* execution/ex_utils_2.c */
int		set_exit_status(int num_of_p, t_mini *mini, int *pids);
void	ses_help(t_mini *mini, int *signaled, int *status, int *last_sig);
void	ses_init(int *signaled, int *i, int *status, int *last_sig);

/* execution/ex_files_pipes.c */
int		close_files(int *infile, int *outfile);
int		close_all_pipes(int **pipes, int pipe_count);
int		open_pipes(int **pipes, int process_count);
int		set_files(t_mini *mini, t_cmd *nthcmd, int *infile, int *outfile);

/* execution/ex_stdin_stdout.c */
void	set_first_process(int files[], int num_of_p, int **pipes);
void	set_mid_processes(int files[], int num_of_p, int **pipes, int i);
void	set_last_process(int files[], int num_of_p, int **pipes, int i);
int		set_ins_outs(int i, int **pipes, int files[2], int num_of_p);
void	init_fqoe(char *q_start, int *i, int *q_ign, int *dollar_quote);

/* lexer/lexer_quotes.c */
int		find_words(char *text);
char	**trim_quotes_in_array(char **head);
char	*str_from_array(char **head);
char	**process_envs_and_quotes(t_mini *mini, t_token *token);
int		check_consecutive_quotes(char *txt);

/* lexer/lexer_quotes_utils.c */
char	*exp_sub(t_mini *mini, char *str);
int		check_next_char(char c, char c2, int i);
int		check_dollar_sq(char *str);
char	*strdup_from_second(const char *str);
char	**peaq_help(void);

/* lexer/lexer_utils.c */
void	init_gtl_vars(int *f, int *i, char **node, t_token **token);
int		cnc_check(char *text, int *i);
int		find_q_or_end(char *text);
void	set_q_ign(int *q_ign, int value);
int		find_q_helper(char q_start, char *text, int i, int *q_ign);

/* lexer/lexer.c */
int		create_and_add_tok(t_mini *mini, char *node_value,
			t_token **token_list, int *hdoc);
char	*process_text(char *text, int *i, int in_sq, int in_dq);
char	*create_node_value(char *input, int *i);
t_token	*get_token_list(t_mini *mini, char *input);
int		lexer(char *input, t_mini *mini);

/* parser/heredoc_utils.c */
int		heredoc_dup(t_mini *mini);
void	heredoc_handler(int sig);
char	*heredoc_expand(t_mini *mini, char *str);

/* parser/parser_heredoc.c */
char	*trim_quotes_in_str(char *str, int *expand_flag, t_mini *mini);
char	*heredoc_readline(t_mini *mini, char *limit, int *expand_flag);
char	*make_new_limit(char *limit, int *expand_flag, t_mini *mini);
int		process_heredoc(t_token *token, char *limit, t_mini *mini);
int		parser_heredoc(t_mini *mini);

/* parser/parser.c */
char	**alloc_args(char **args, t_token *token);
int		first_entry(t_token **token, t_cmd **node, char ***args, char ***ahead);
int		parser(t_mini *mini);

/* types/array.c */
char	**dup_array(char **arr);
void	free_arr(char **arr);
int		get_arr_len(char **arr);
char	**add_back_array(char **arr, char *new_el);
char	**change_arr_element(char **arr, char *new_el, int index);

/* types/t_cmd.c */
void	init_cmd_node(t_cmd *node);
int		nc_init(t_cmd **node, char ***args, char ***ahead);
t_cmd	*new_cmd(t_token *token);
void	add_back_cmd(t_cmd **lst, t_cmd *new);
void	free_cmd_list(t_mini *mini);
void	free_cmd_nodes(t_cmd *cmd);
t_cmd	*get_nth_command(t_cmd *cmdhead, int n);
int		get_cmd_count(t_cmd *cmd);

/* types/t_redir.c */
int		new_redir_condition(t_token *token);
t_redir	*create_redir(t_type type, char *value);
void	add_back_redir(t_redir **lst, t_redir *new);
t_redir	*find_redirs(t_token *token);
void	free_redir(t_redir *redir);

/* types/t_token.c */
t_type	get_token_type(char *value);
t_token	*init_new_token(char *value, t_type type);
void	add_back_token(t_token **lst, t_token *new);

/* types/t_token_utils.c */
t_token	*remove_null_tokens(t_token *token);
int		get_ttokens_len(t_token	*token);
void	free_token_list(t_mini *mini);

/* utils/file_utils.c */
char	*get_current_directory(void);
int		is_directory(const char *path);
int		is_executable_file(const char *path);
int		is_readable_file(const char *path);
int		is_writable_file(const char *path);

/* utils/free.c */
void	free_memo(void *mem_seg);
void	free_four_mallocs(char *s1, char *s2, char *s3, char *s4);

/* utils/messages_2.c */
void	s_error_msg(char *msg);
int		mini_perror(t_mini *mini, char *msg);
int		mini_error(t_mini *mini, char *msg, int err);

/* utils/messages.c */
int		error_msg(char *msg, t_mini *mini, char *str_1, char *str_2);
char	*append_word(char *msg, char *word);
char	*create_msg(char *first, char *second, char *third, char *fourth);

/* utils/signal.c */
void	sig_handler(int sig);
void	sigint_void(int sig);

/* utils/testing.c */
void	print_token_list(t_mini *mini);
void	print_command_list(t_mini *mini);

/* types/token_list.c */
t_token	*init_new_token(char *value, t_type type);
void	add_back_token(t_token **lst, t_token *new);
t_token	*remove_null_tokens(t_token *token);

/* utils/utils.c */
int		iswhitespace(char c);
int		is_alnum(char *str);
char	*str_append_nl(char *s1, char *s2);
char	*str_append_space(char *s1, char *s2);
int		array_char_len(char **head);

#endif