/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/10/29 17:11:48 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*process_input(char *input) // should be void
{
	t_list	*token_list;
	
	token_list = NULL;
	token_list = lexer(input);

	return (token_list); // testing 
}

// called on loop to show a prompt
int	show_prompt()
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
	printf("%s\n", input); // for now just print
	add_history(input);

	/* testing lexer */
	// t_list *token_list = process_input(input);
	// printf("tokens from input:\n");
	// while (token_list)
	// {
	// 	printf("%s\n", token_list->content); // attention content
	// 	token_list = token_list->next;
	// }	
		
	free(input);
	return (1);
}

void	set_termios() // terminal config editing to prevent '^\' from being printed 
{
	struct termios	termios;

	if (tcgetattr(0, &termios) == -1) // gets stdin settings, exits if fail
		exit(1);
	termios.c_cc[VQUIT] = _POSIX_VDISABLE; // only blocks '\' bc ctrl c prints ^C in bash
	if ((tcsetattr(0, TCSANOW, &termios)) == -1) // sets stdin settings immediately
		exit(1);
}

int main(int argc, char *argv[], char *env[])
{
	signal(SIGINT, sig_handler); // ctrl c
	signal(SIGQUIT, sig_handler); // ctrl '\'
	(void)argv; (void)env;
	set_termios();
	if (argc != 1)
		error_msg("Too many arguments. Use: ./minishell");
	while (1)
		if (show_prompt() == 0) // if ctrl d, break the loop, clear history and return
			break ;
	rl_clear_history();
	return (0);
}