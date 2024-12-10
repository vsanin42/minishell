/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:40:14 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/10 16:19:42 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_sig_received = 0; // not really useful now but keep it

// default handler function definition in sigaction struct, must return void.
// if it doesn't work for some reason, use global variable
// it gets set in the if statements, indicating the type of signal received
// everything else (everything inside ifs) happens outside. for now keep this
// SIGINT means ctrl c was pressed sot hen we:
// tell readline that the input has moved onto \n
// rl_replace_line("", 0) makes input empty (removes everything written so far)
// display the line again
// reset g_gig_sigreceiver to 0
void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig_received = 0;
	}
}

	// else if (sig == SIGQUIT)
	// {
	// 	rl_on_new_line();
	// 	rl_replace_line(rl_line_buffer, 0);
	// 	rl_redisplay();
	// 	g_sig_received = 0; // reset
	// }

void	sigint_void(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}
