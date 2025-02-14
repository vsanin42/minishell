/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/18 13:03:51 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sig = 0;

// executes lexer->parser->executor with checks between each phase
// @returns 0 on SUCESS, 1 on ERROR
// print_token_list(mini);
// print_command_list(mini);
// possible result codes: 0, 1, 2, 126, 127 (128+ for signals)
int	process_input(char *input, t_mini *mini)
{
	int	result;

	result = lexer(input, mini);
	if (result != 0)
		return (free_token_list(mini), result);
	mini->token_list = remove_null_tokens(mini->token_list);
	if (mini->token_list == NULL)
		return (0);
	result = token_evaluator(mini);
	if (result != 0)
		return (free_token_list(mini), result);
	result = parser_heredoc(mini);
	if (result != 0)
		return (free_token_list(mini), result);
	result = parser(mini);
	if (result != 0)
		return (free_token_list(mini), free_cmd_list(mini), result);
	free_token_list(mini);
	result = executor(mini, get_cmd_count(mini->cmd_list));
	free_int_arr(mini->pipes, mini->pids);
	free_cmd_list(mini);
	return (result);
}

// called in loop to show a prompt and process input
// @returns 1 so the calling loop can continue, 0 on error so loop breaks
// (!input) = called when ^C was pressed - no input was received
// input[0] == '\0' - input was empty or just enter
// \033[32mminishell\033[37m>
// here we are done with the previous command
// because all possible exits from the command lead here
// so we set result of process_input to mini->exit_status
// so next command can access it
int	show_prompt(t_mini *mini)
{
	char	*input;

	mini->pids = NULL;
	mini->pipes = NULL;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	set_termios(1);
	input = readline("minishell$ ");
	if (g_sig == SIGINT)
	{
		mini->exit_status = g_sig + 128;
		g_sig = 0;
	}
	if (!input)
		return (-1);
	if (input[0] == '\0')
	{
		free(input);
		return (1);
	}
	add_history(input);
	if (check_input(input, mini) == 1)
		return (free(input), 1);
	mini->exit_status = process_input(input, mini);
	return (mini->exit_status);
}

// terminal config editing to revent '^\' from being printed
// tcgetattr gets stdin settings, exits if fail
// termios.c_cc[VQUIT]=_POSIX_VDISABLE disables ctrl '\'
// if mode is 0, it enables ctrl '\' - for child processes
// tcsetattr sets stdin settings immediately
void	set_termios(int mode)
{
	struct termios	termios;

	if (tcgetattr(0, &termios) == -1)
		exit(ERROR);
	if (mode)
		termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	else
		termios.c_cc[VQUIT] = 28;
	if ((tcsetattr(0, TCSANOW, &termios)) == -1)
		exit(ERROR);
}

// mini = (t_mini *)malloc(sizeof(t_mini)); // ??? why this causes leaks
// if (!mini)
// 	return ;
void	init_mini(t_mini *mini, char **env)
{
	mini->token_list = NULL;
	mini->cmd_list = NULL;
	mini->exit_status = 0;
	mini->env = dup_array(env);
	mini->pids = NULL;
	mini->pipes = NULL;
}

// SIGINT ctrl '\'
// SIGQUIT ctrl 'C'
// if ctrl D or program returns 0, break the loop, clear history and return
int	main(int argc, char *argv[], char *env[])
{
	t_mini	mini;
	int		stdin;
	int		stdout;

	g_sig = 0;
	(void)argv;
	(void)argc;
	if (argc != 1)
		return (s_error_msg("Too many arguments. Use: ./minishell"), ERROR);
	init_mini(&mini, env);
	stdin = dup(STDIN_FILENO);
	stdout = dup(STDOUT_FILENO);
	while (1)
	{
		dup2(stdin, 0);
		dup2(stdout, 1);
		if (show_prompt(&mini) == -1)
			break ;
	}
	write(1, "exit\n", 5);
	free_arr(mini.env);
	rl_clear_history();
	return (0);
}
