/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_files_pipes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:50:53 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/07 18:56:21 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// can closing go wrong?
int	close_files(int *infile, int *outfile)
{
	if (*infile > STDIN_FILENO)
		close(*infile);
	if (*outfile > STDOUT_FILENO)
		close(*outfile);
	return (0);
}

// can closing go wrong?

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

int	open_pipes(int pipes[][2], int process_count)
{
	int	i;
	int result;

	i = 0;
	result = 0;
	while (i < process_count - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			result = errno;
			perror("minishell");
			close_all_pipes(pipes, i);
			return (result);
		}
		i++;
	}
	return (0);
}

int	set_ins_outs(int i, int pipes[][2], int files[2], int num_of_p)
{
	if (i == 0)
	{
		dup2(files[0], STDIN_FILENO);
		if (num_of_p - 1 > 0) // so if we have no pipe open when only 1 process we dont use pipe WR end - not working, still writes to pipe
		{
			if (files[1] > STDOUT_FILENO)
				dup2(files[1], STDOUT_FILENO);
			else
				dup2(pipes[0][1], STDOUT_FILENO);
		}
	}
	// set ends of pipes to middle processes
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

int	set_files(t_mini *mini, t_cmd *nthcmd, int *infile, int *outfile)
{
	t_redir	*redir;
	int temp_pipe[2];
	int result;

	redir = nthcmd->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRIN)
		{
			if (*infile > STDIN_FILENO)
				close(*infile);
			*infile = open(redir->file, O_RDONLY);
			if (*infile == -1)
				return (mini_perror(mini));
			// it can go wrong? maybe set errorcode and return it
		}
		else if (redir->type == TOKEN_HEREDOC)
		{
			if (pipe(temp_pipe) == -1)
			{
				result = errno;
				perror("minishell");
				return(result);
			}
			write(temp_pipe[1], redir->file, ft_strlen(redir->file));
			close(temp_pipe[1]);
			dup2(temp_pipe[0], STDIN_FILENO);
			close(temp_pipe[0]);
		}
		else if (redir->type == TOKEN_REDIROUT)
		{
			if (*outfile > STDOUT_FILENO)
				close(*outfile);
			*outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (redir->type == TOKEN_APPEND)
		{
			if (*outfile > STDOUT_FILENO)
				close(*outfile);
			*outfile = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		if (*infile == -1)
			return (mini_perror(mini));
		if (*outfile == -1)
			return (mini_perror(mini));
		redir = redir->next;
	}
	return (0);
}
