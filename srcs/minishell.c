/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/06 18:57:02 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// executes lexer->parser->executor with checks between each phase
// @returns 0 on SUCESS, 1 on ERROR
int	process_input(char *input, t_mini *mini)
{
	if (lexer(input, mini) == ERROR)
		return (free_token_list(mini), ERROR);
	mini->token_list = remove_null_tokens(mini->token_list);
	if (token_evaluator(mini) == ERROR)
		return (free_token_list(mini), ERROR);
	//print_token_list(mini);
	if (parser_heredoc(mini) == ERROR)
		return (free_token_list(mini), ERROR);
	if (parser(mini) == ERROR)
		return (free_token_list(mini), free_cmd_list(mini), ERROR);
	free_token_list(mini);
	//print_command_list(mini);
	if (cmd_evaluator(mini) == 0)
	{
		if (executor(mini) == ERROR)
			return (free_cmd_list(mini), ERROR);
	}
	free_cmd_list(mini);
	return (0);
}

// called in loop to show a prompt and process input
// @returns 1 so the calling loop can continue, 0 on error so loop breaks
// (!input) = called when ^C was pressed - no input was received
// input[0] == '\0' - input was empty or just enter
int	show_prompt(t_mini *mini)
{
	char	*input;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	set_termios(1);
	input = readline("\033[32mminishell \033[37m> ");
	if (!input)
		return (0);
	if (input[0] == '\0')
	{
		free(input);
		input = NULL;
		return (1);
	}
	add_history(input);
	if (check_input(input) == 1)
		return (free(input), 1);
	process_input(input, mini);
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
	//mini->error_msg = NULL;
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
	while (1)
	{
		if (show_prompt(&mini) == 0)
			break ;
	}
	write(1, "exit\n", 5);
	free_arr(mini.env);
	rl_clear_history();
	return (mini.exit_status);
}
