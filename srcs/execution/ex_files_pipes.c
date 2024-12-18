/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_files_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 15:50:53 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/18 11:23:02 by zpiarova         ###   ########.fr       */
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
int	close_all_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count - 1)
	{
		close(pipes[i][1]);
		close(pipes[i][0]);
		i++;
	}
	return (0);
}

int	open_pipes(int **pipes, int process_count)
{
	int	i;
	int	result;

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

void	set_files_helper(int *file, t_redir *redir, int std_fd)
{
	if (*file > std_fd)
		close(*file);
	if (redir->type == TOKEN_REDIRIN)
		*file = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_REDIROUT)
		*file = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_APPEND)
		*file = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
}

int	set_files(t_mini *mini, t_cmd *nthcmd, int *infile, int *outfile)
{
	t_redir	*redir;
	int		temp_pipe[2];

	redir = nthcmd->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRIN)
			set_files_helper(infile, redir, STDIN_FILENO);
		else if (redir->type == TOKEN_HEREDOC)
		{
			if (pipe(temp_pipe) == -1)
				return (perror("minishell"), errno);
			*infile = temp_pipe[1];
			write(temp_pipe[1], redir->file, ft_strlen(redir->file));
			close(temp_pipe[1]);
			dup2(temp_pipe[0], STDIN_FILENO);
			close(temp_pipe[0]);
		}
		else if (redir->type == TOKEN_REDIROUT || redir->type == TOKEN_APPEND)
			set_files_helper(outfile, redir, STDOUT_FILENO);
		if (*infile == -1 || *outfile == -1)
			return (mini_perror(mini, NULL));
		redir = redir->next;
	}
	return (0);
}
