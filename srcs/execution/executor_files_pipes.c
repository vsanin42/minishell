/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_files_pipes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:50:53 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/05 13:37:16 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// closes files taken as parameters
int	close_files(int *infile, int *outfile)
{
	if (*infile > STDIN_FILENO)
		close(*infile);
	if (*outfile > STDOUT_FILENO)
		close(*outfile);
	return (0);
}

// closes all opened pipes
int	close_all_pipes(int pipes[][2], int pipe_count)
{
	int i;

	i = 0;
	while (i < pipe_count - 1)
	{
		close(pipes[i][1]);
		close(pipes[i][0]);
		i++;
	}
	return (0);
}

// for n processes, opens n - 1 pipes
// @returns 0 on success, 1 on error
int	open_pipes(int pipes[][2], int process_count)
{
	int	i;

	i = 0;
	while (i < process_count - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			printf("error creating pipes");
			close_all_pipes(pipes, i);
			return (ERROR);
		}
		i++;
	}
	return (0);
}

// sets STDIN and STDOUT of each child process to pipes/files, if there are any
// 1st process: STDIN is infile, STDOUT is first pipe or outfile if n = 1
// middle processes: pipe[wr]
// !if there is redir file specified for process, it uses this instead of pipe
int	set_ins_outs(int i, int pipes[][2], int files[2], int num_of_p)
{
	if (i == 0)
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
	if (i > 0 && i < num_of_p - 1)
	{
		if (files[0] > STDIN_FILENO)
			dup2(files[0], STDIN_FILENO);
		else
			dup2(pipes[i - 1][0], STDIN_FILENO);
		if (files[1] > STDOUT_FILENO)
			dup2(files[1], STDOUT_FILENO);
		else
			dup2(pipes[i][1], STDOUT_FILENO);
	}
	// set up outfile - read end of last pipe
	if (i == num_of_p - 1)
	{
		if (num_of_p - 1 > 0) // so if we have no pipe open when only 1 process we dont use pipe RD end - not working, still reads from pipe
		{
			if (files[0] > STDIN_FILENO)
				dup2(files[0], STDIN_FILENO);
			else
				dup2(pipes[i - 1][0], STDIN_FILENO);
		}
		dup2(files[1], STDOUT_FILENO);
	}
	return (0);
}

// finds and sets in/outfile for each process so it knows where to get input
// if no redir is fount for that process it keeps
int	set_files(t_cmd *nthcmd, int *in, int *out)
{
	t_redir	*redir;

	redir = nthcmd->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRIN)
		{
			if (*in > STDIN_FILENO)
				close(*in);
			*in = open(redir->file, O_RDONLY);
			// it can go wrong? maybe set errorcode and return it
		}
		else if (redir->type == TOKEN_REDIROUT || redir->type == TOKEN_APPEND)
		{
			if (*out > STDOUT_FILENO)
				close(*out);
			if (redir->type == TOKEN_REDIROUT)
				*out = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (redir->type == TOKEN_APPEND)
				*out = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			// it can go wrong? maybe set errorcode and return it
		}
		// else if (redir->type == TOKEN_REDIROUT)
		// {
		// 	if (*out > STDOUT_FILENO)
		// 		close(*out);
		// 	*out = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		// 	// it can go wrong? maybe set errorcode and return it
		// }
		// else if (redir->type == TOKEN_APPEND)
		// {
		// 	if (*out > STDOUT_FILENO)
		// 		close(*out);
		// 	*out = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		// 	// it can go wrong? maybe set errorcode and return it
		// }
		redir = redir->next;
	}
	return (0);
}
