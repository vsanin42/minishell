/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:40:14 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/10 13:15:00 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1); // printf is not async-signal-safe, better use write
		rl_on_new_line(); // tell readline that the input has moved onto \n
		rl_replace_line("", 0); // make the input empty (removes whatever has been written so far)
		rl_redisplay(); // display the line again
	}
}

void	sigint_void(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}
