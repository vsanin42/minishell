/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/09 22:35:13 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// executes lexer->parser->executor with checks between each phase
// @returns 0 on SUCESS, 1 on ERROR
int	process_input(char *input, t_mini *mini)
{
	int	result;

	result = lexer(input, mini);
	if (result != 0)
		return (free_token_list(mini), ERROR);
	mini->token_list = remove_null_tokens(mini->token_list);
	result = token_evaluator(mini);
	if (result != 0)
		return (free_token_list(mini), ERROR);
	//print_token_list(mini);
	result = parser_heredoc(mini);
	if (result != 0)
		return (free_token_list(mini), ERROR);
	result = parser(mini);
	if (result != 0)
		return (free_token_list(mini), free_cmd_list(mini), ERROR);
	free_token_list(mini);
	//print_command_list(mini);
	if (cmd_evaluator(mini) == 0) // if we leave it out, open cares for some edge cases but not for all - eg. it sets errno if file does not exist but not if it is directoryy when we expect file - maybe can leave out after some work
		result = executor(mini);
	free_cmd_list(mini);
	return (result);
}

// called in loop to show a prompt and process input
// @returns 1 so the calling loop can continue, 0 on error so loop breaks
// (!input) = called when ^C was pressed - no input was received
// input[0] == '\0' - input was empty or just enter
int	show_prompt(t_mini *mini)
{
	char	*input;
	int result = 0;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	set_termios(1);
	input = readline("minishell>"); // \033[32mminishell\033[37m>
	if (!input)
		return (-1);
	if (input[0] == '\0')
	{
		free(input);
		input = NULL;
		return (1);
	}
	add_history(input);
	if (check_input(input, mini) == 1)
		return (free(input), 1);
	result = process_input(input, mini);
	return (1);
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

void	init_mini(t_mini *mini, char **env)
{
	// mini = (t_mini *)malloc(sizeof(t_mini)); // ??? why this causes leaks
	// if (!mini)
	// 	return ;
	mini->token_list = NULL;
	mini->cmd_list = NULL;
	mini->exit_status = 0;
	mini->env = dup_array(env);
}

// SIGINT ctrl '\'
// SIGQUIT ctrl 'C'
// if ctrl D or program returns 0, break the loop, clear history and return
int	main(int argc, char *argv[], char *env[])
{
	t_mini	mini;

	(void)argv;
	if (argc != 1)
		return (s_error_msg("Too many arguments. Use: ./minishell"), ERROR);
	init_mini(&mini, env);
	int stdin = dup(STDIN_FILENO);
	int stdout = dup(STDOUT_FILENO);
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
