/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_files_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:50:53 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/11 13:45:57 by zuzanapiaro      ###   ########.fr       */
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

int	set_files(t_mini *mini, t_cmd *nthcmd, int *infile, int *outfile)
{
	t_redir	*redir;
	int		temp_pipe[2];
	int		result;

	redir = nthcmd->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRIN)
		{
			if (*infile > STDIN_FILENO)
				close(*infile);
			*infile = open(redir->file, O_RDONLY);
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
			return (mini_perror(mini, NULL));
		if (*outfile == -1)
			return (mini_perror(mini, NULL));
		redir = redir->next;
	}
	return (0);
}
