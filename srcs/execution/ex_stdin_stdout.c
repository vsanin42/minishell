/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_stdin_stdout.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:29:11 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/16 20:27:41 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_first_process(int files[], int num_of_p, int **pipes)
{
	dup2(files[0], STDIN_FILENO);
	if (num_of_p - 1 > 0)
	{
		if (files[1] > STDOUT_FILENO)
			dup2(files[1], STDOUT_FILENO);
		else
			dup2(pipes[0][1], STDOUT_FILENO);
	}
}

void	set_mid_processes(int files[], int num_of_p, int **pipes, int i)
{
	(void)num_of_p;
	if (files[0] > STDIN_FILENO)
		dup2(files[0], STDIN_FILENO);
	else
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (files[1] > STDOUT_FILENO)
		dup2(files[1], STDOUT_FILENO);
	else
		dup2(pipes[i][1], STDOUT_FILENO);
}

void	set_last_process(int files[], int num_of_p, int **pipes, int i)
{
	if (num_of_p - 1 > 0)
	{
		if (files[0] > STDIN_FILENO)
			dup2(files[0], STDIN_FILENO);
		else
			dup2(pipes[i - 1][0], STDIN_FILENO);
	}
	dup2(files[1], STDOUT_FILENO);
}

// set up STDIN and STDOUT of each process
// if there is a file sets it to file
// else it sets it to proper ends of pipes
// in first process, STDIN is infile or STDIN
// in last process, STDOUT is outfile or STDOUT
int	set_ins_outs(int i, int **pipes, int files[2], int num_of_p)
{
	if (i == 0)
		set_first_process(files, num_of_p, pipes);
	if (i > 0 && i < num_of_p - 1)
		set_mid_processes(files, num_of_p, pipes, i);
	if (i == num_of_p - 1)
		set_last_process(files, num_of_p, pipes, i);
	return (0);
}

// no space in lexer files
void	init_fqoe(char *q_start, int *i, int *q_ign, int *dollar_quote)
{
	*dollar_quote = 0;
	*q_ign = 0;
	*i = 0;
	*q_start = '\0';
}
