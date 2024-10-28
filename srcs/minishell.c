/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:52:10 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/10/28 13:34:08 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sig_received = 0; // not really useful now but keep it

// default handler function definition in sigaction struct, must return void.
// if it doesn't work for some reason, use global variable
// it gets set in the if statements, indicating the type of signal received
// everything else (everything inside the ifs) happens outside. for now keep this
void	sighandle(int sig)
{
	if (sig == SIGINT) // if ctrl c was pressed
	{
		write(1, "\n", 1); // printf is not async-signal-safe, better use write
		rl_on_new_line(); // tell readline that the input has moved onto \n
		rl_replace_line("", 0); // make the input empty (removes whatever has been written so far)
		rl_redisplay(); // display the line again
		g_sig_received = 0; // reset
	}
	else if (sig == SIGQUIT) // ctrl '\' - same here but no newline
	{
		rl_on_new_line();
		rl_replace_line(rl_line_buffer, 0); // same but keeps what has been written so far
		rl_redisplay();
		g_sig_received = 0; // reset
	}
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
	free(input);
	return (1);
}

// might need a better way to exit later because this doesn't free anything
int	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
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
	signal(SIGINT, sighandle); // ctrl c
	signal(SIGQUIT, sighandle); // ctrl '\'
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