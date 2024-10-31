/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:40:14 by vsanin            #+#    #+#             */
/*   Updated: 2024/10/31 16:17:02 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sig_received = 0; // not really useful now but keep it

// default handler function definition in sigaction struct, must return void.
// if it doesn't work for some reason, use global variable
// it gets set in the if statements, indicating the type of signal received
// everything else (everything inside the ifs) happens outside. for now keep this
void	sig_handler(int sig)
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