/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/11 19:53:04 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*process_input(char *input, t_mini *mini) // should be void
{
	mini->token_list = lexer(input);
	print_token_list(mini);
	// if (mini->cmd_list)
	// 	mini->cmd_list =  parser(mini); ??? why if mini??? it didnt run then :(
	mini->cmd_list =  parser(mini);
	free_token_list(mini->token_list);
	print_command_list(mini);
	free_cmd_list(mini->cmd_list);
	return (mini->cmd_list); // testing
}

// called on loop to show a prompt
int	show_prompt(t_mini *mini)
{
	char	*input;

	input = readline("\033[32mminishell \033[37m> "); // colors optional but looks nicer
	if (!input) // if ctrl d was pressed, exit the process
		return (0); // needs proper exit in the future
	if (input[0] == '\0') // if empty input/enter
	{
		free(input);
		return (1); // if enter (empty input) was pressed, continue to the next iteration
	}
	//printf("%s\n", input); // for now just print
	add_history(input);
	process_input(input, mini);
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

	/* testing lexer */
	// t_token *head = NULL;
	// t_token *token_list = process_input(input, mini);
	// head = token_list;

	/* testing parser */
	//t_cmd *head = NULL;
	// t_cmd *cmd_list = process_input(input, mini);
	//head = cmd_list;

	// printf("tokens from input:\n");
	// while (token_list)
	// {
	// 	printf("%s\t", token_list->value); // attention content
	// 	printf("type: %d\n", token_list->type);
	// 	token_list = token_list->next;
	// }

	/* testing parser */
	// printf("commands:\n\n");
	// t_cmd *temp = cmd_list;
	// char **atemp2;
	// t_redir *aredir;
	// while (temp)
	// {
	// 	printf("cmd name:\t%s\n", temp->cmd);
	// 	atemp2 = temp->args;
	// 	aredir = temp->redir;
	// 	while (atemp2 && *atemp2)
	// 	{
	// 		printf("argument:\t%s\n", *atemp2);
	// 		atemp2++;
	// 	}
	// 	while (aredir)
	// 	{
	// 		printf("redir file:\t%s\n", aredir->file);
	// 		printf("redir type:\t%d\n", aredir->type);
	// 		aredir = aredir->next;
	// 	}
	// 	printf("---------------------------\n");
	// 	temp = temp->next;
	// }
	//free_cmd_list(head);


	/* testing paring envs and quotes to true text values */
	// printf("parsing:\n");
	// t_token *temp = token_list;
	// while (temp)
	// {
	// 	parse_envs_and_quotes(temp);
	// 	temp = temp->next;
	// 	printf("\n");
	// }
	//clear_cmd_list(head);
	/* testing lexer */
	// printf("tokens:\n");
	// t_token *temp = token_list;
	// char	*new_str;
	// while (temp)
	// {
	// 	printf("\n");
	// 	//printf("old value:\t%s\n", temp->value);
	// 	new_str = parse_eq(temp);
	// 	free(temp->value);
	// 	temp->value = new_str;
	// 	//printf("new value:\t%s\n", temp->value);
	// 	temp = temp->next;
	// }
	//clear_token_list(head);

	/* printing token list */
	// t_token *temp2 = token_list;
	// while (temp2)
	// {
	// 	printf("%s\t", temp2->value); // attention content
	// 	printf("type: %d\n", temp2->type);
	// 	temp2 = temp2->next;
	// }
	// clear_token_list(head);
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
