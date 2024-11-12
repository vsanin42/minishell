/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/12 19:50:16 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*process_input(char *input, t_mini *mini) // should be void
{
	mini->token_list = lexer(input);
	print_token_list(mini);
	// if (mini->cmd_list)
	// 	mini->cmd_list =  parser(mini);    why if mini??? it didnt run then :(
	mini->cmd_list =  parser(mini);
	free_token_list(mini->token_list);
	print_command_list(mini);
	free_cmd_list(mini->cmd_list);
	return (mini->cmd_list); // testing
}

// called in loop to show a prompt and proessits input
int	show_prompt(t_mini *mini)
{
	char	*input;

	input = readline("\033[32mminishell \033[37m> ");
	if (!input) // if ctrl d was pressed, exit the process
		return (0); // needs proper exit in the future
	if (input[0] == '\0') // if empty input/enter
	{
		free(input);
		return (1); // if enter (empty input) was pressed, continue to the next iteration
	}
	add_history(input);
	process_input(input, mini); // called without assigning, just for testing


	/* 	testing current directory - WORKS */
	/* char *cwd = get_current_directory(); // testing finding a path when we will be expecting path type
		printf("cwd: %s\n", cwd);
		free(cwd); */

	/*	testing getting command paths - WORKS */
	/* 	char *path = get_path_env(input);
	if (path)
		printf("path: %s\n", path);
	else
		printf("Path does not exist"); */

	/* testing cd_builtin - WORKS  */
	/* 	printf("cd: %d\n", cd_builtin(input));
	char *cwd = get_current_directory(); // testing finding a path when we will be expecting path type
	printf("cwd: %s\n", cwd);
	free(cwd); */


	/* 	testing builtin pwd with no options - WORKS
	pwd_builtin(); */

	/* 	testing exit  - WORKS */
	/* 	if (input)
		exit_builtin(input); */


	/* testing is_executable/is_readable - WORKS
	printf("exec: %d\n", is_readable_file(input)); */

	/* testing if input is being redirected properly - WORKS  */
	/* 	if (redirect_input(input) == -1)
	{
		perror("Error redirecting input hh");
		return 1;  // Exit or handle error appropriately
	}
	char buff[50];
	ssize_t bytesRead = read(STDIN_FILENO, buff, sizeof(buff) - 1);
	if (bytesRead == -1) {
		perror("Error reading from redirected input");
		return 1;  // Handle the read error appropriately
	}
	buff[bytesRead] = '\0';  // Null-terminate the string to safely print
	printf("buff: %s\n", buff); */

	return (1);
}

void	set_termios() // terminal config editing to revent '^\' from being printed
{
	struct termios	termios;

	if (tcgetattr(0, &termios) == -1) // gets stdin settings, exits if fail
		exit(1);
	termios.c_cc[VQUIT] = _POSIX_VDISABLE; // only blocks '\' bc ctrl c prints ^C in bash
	if ((tcsetattr(0, TCSANOW, &termios)) == -1) // sets stdin settings immediately
		exit(1);
}

// init_mini(t_mini *mini)
// {
// 	env =
// }

int main(int argc, char *argv[], char *env[])
{
	t_mini	mini;

	//mini = NULL;
	mini.env = env;
	mini.token_list = NULL;
	mini.cmd_list = NULL;
	//init_mini(&mini);
	signal(SIGINT, sig_handler); // ctrl c
	signal(SIGQUIT, sig_handler); // ctrl '\'
	(void)argv; (void)env;
	set_termios();
	if (argc != 1)
		error_msg("Too many arguments. Use: ./minishell", NULL, NULL, NULL);
	while (1)
		if (show_prompt(&mini) == 0) // if ctrl d, break the loop, clear history and return
			break ;
	rl_clear_history();
	return (0);
}
