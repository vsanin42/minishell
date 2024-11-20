/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/20 11:32:48 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_cmd_count(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		i += 1;
		cmd = cmd->next;
	}
	return (i);

}

t_cmd *get_nth_command(t_cmd *cmdhead, int n)
{
	int	i;

	i = 0;
	if (!cmdhead)
	{
		return (NULL);
	}
	while (i < n && cmdhead)
	{
		cmdhead = cmdhead->next;
		i++;
	}
	if (i == n)
		return (cmdhead);
	else
		return (NULL);
}

// NOT WORKING - testing function for n pipes
int	executorr(t_mini *mini, t_cmd *cmd)
{
	int		infile;
	int		outfile;
	int		i;
	int		j;
	t_redir	*redir;
	pid_t	pid;
	// (void)mini;
	int	cmdcount = get_cmd_count(cmd);
	int	pipes[cmdcount - 1][2];
	char *path = NULL;

	// opening all pipes - then must connect them properly
	i = 0;
	while (i < cmdcount - 1)
	{
		pipe(pipes[i]);
		i++;
	}

	// opening files - only the last one is valid always - BUT WE HAVE TO GO THROUGH ALL REDIRS OF ALL CMDS, NOT ONLY THE FIRST ONE
	redir = cmd->redir;
	printf("red: %s\n", cmd->redir->file);
	if (cmd->redir == NULL)
		printf("redir: NULL\n");
	else
		printf("redirrrr\n");
	while (redir)
	{
		// redir in
		if (infile == -1 && redir->type == TOKEN_REDIRIN)
			infile = open(redir->file, O_RDONLY);
		else if (infile != -1 && redir->type == TOKEN_REDIRIN)
		{
			close(infile);
			infile = open(redir->file, O_RDONLY);
		}
		// redir out
		else if (outfile == -1 && redir->type == TOKEN_REDIROUT)
			outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (outfile != -1 && redir->type == TOKEN_REDIROUT)
		{
			close(outfile);
			outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		// redir append
		else if (outfile == -1&& redir->type == TOKEN_APPEND)
			outfile = open(redir->file, O_WRONLY | O_CREAT, 0644);
		else if (outfile != -1 && redir->type == TOKEN_APPEND)
		{
			close(outfile);
			outfile = open(redir->file, O_WRONLY | O_CREAT, 0644);
		}
		// set heredoc stream from stdin
		redir = redir->next;
	}
	printf("cmd count: %d\n", cmdcount);
	i = 0;
	while (i < cmdcount && cmd)
	{
		j = 0;
		path = get_path_env(cmd->cmd);
		if (!path)
		{
			printf("%s: command not found\n", cmd->cmd);
			return (ERROR);
		}
		printf("i: %d\n", i);
		printf("cmd: %s\n", path);
		free(path);
		path = NULL;
		pid = fork();
		if (pid == 0)
		{
			printf("forked: child %d\n", pid);
			// first command
			if (i == 0 && infile)
			{
				dup2(infile, STDIN_FILENO);
				close(infile);
			}
			// last command
			if (i == cmdcount - 1 && outfile)
			{
				dup2(outfile, STDOUT_FILENO);
				close(outfile);
			}
			// set pipes for all commads between pipes
			if (i > 0 && i < cmdcount)
				dup2(pipes[i - 1][1], STDIN_FILENO);
			if (i < cmdcount - 1)
				dup2(pipes[i][0], STDOUT_FILENO);
			// close all pipes in each child
			while (j < cmdcount - 1)
			{
				close(pipes[j][0]);
				close(pipes[j - 1][1]);
				j++;
			}
			// perform action
			if (execve(path, cmd->args, mini->env) == -1)
			{
				printf("%s: could not execute command\n", cmd->cmd);
				return (ERROR);
			}
		}
		cmd = cmd->next;
		i++;
	}
	printf("checked commands\n");
	i = 0;
	// parent waits for all children
	while (i < cmdcount)
	{
		wait(NULL);
		i++;
	}
	printf("waited for children\n");
	// close all fds
	if (infile)
		close(infile);
	printf("closed infile\n");
	if (outfile)
		close(outfile);
	printf("closed outfile\n");
	// close all pipes in parent
	i = 0;
	while (i < cmdcount - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	printf("closed pipes\n");

	return (0);
}

// works for one command without pipes -> eg. < infile.txt sort >> outfile.txt
int	executor(t_mini *mini, t_cmd *cmd)
{
	int		pid;
	int		infile = -1;
	int		outfile = -1;
	char	*path;
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRIN)
		{
			if (infile > -1)
				close(infile);
			infile = open(redir->file, O_RDONLY);
		}
		else if (redir->type == TOKEN_REDIROUT)
		{
			if (outfile > -1)
				close(outfile);
			outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (redir->type == TOKEN_APPEND)
		{
			if (outfile > -1)
				close(outfile);
			outfile = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		redir = redir->next;
	}
	pid = fork();
	if (pid == 0)
	{
		if (infile > -1)
		{
			dup2(infile, STDIN_FILENO);
			close(infile);
		}
		if (outfile > -1)
		{
			dup2(outfile, STDOUT_FILENO);
			close(outfile);
		}
		path = get_path_env(cmd->cmd);
		if (execve(path, cmd->args, mini->env) == -1)
			printf("could not execute\n");
	}
	close(infile);
	close(outfile);
	wait(NULL);
	return (0);
}

int	executor_mult(t_mini *mini, t_cmd *cmd)
{
	int		infile = -1;
	int		outfile = -1;
	char	*path;
	t_redir	*redir;
	int num_of_p = get_cmd_count(cmd);
	int		pids[num_of_p];
	int		pipes[num_of_p + 1][2];
	int i;
	int	j;
	t_cmd *nthcmd;

	// open infile and outfile if exist
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRIN)
		{
			if (infile > -1)
				close(infile);
			infile = open(redir->file, O_RDONLY);
		}
		else if (redir->type == TOKEN_REDIROUT)
		{
			if (outfile > -1)
				close(outfile);
			outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (redir->type == TOKEN_APPEND)
		{
			if (outfile > -1)
				close(outfile);
			outfile = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		redir = redir->next;
	}
	// open pipes between each process
	i = 0;
	while (i)
	{
		if (pipe(pipes[i]) == -1)
		{
			printf("error creating pipes");
			return (ERROR);
		}
		i++;
	}
	i = -1;
	// open infile and outfile
	while (++i < num_of_p)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			// go back to parent, there pipes and fds will be closed
			printf("error forking processes\n");
			return (ERROR);
		}
		if (pids[i] == 0)
		{
			// set up infile - write end of first pipe
			if (i == 0 && infile != -1)
			{
				dup2(infile, pipes[0][0]); // or dup2(infile, STDIN_FILENO);
				close(infile);
			}
			else
			{
				dup2(STDIN_FILENO, pipes[0][0]); // or dup2(pipes[0][0], STDIN_FILENO); close(pipes[0][0])

			}
			// set up outfile - read end of last pipe
			if (i == num_of_p + 1 && outfile != -1)
			{
				dup2(outfile, pipes[i][1]);
				close(outfile);
			}
			else
			{
				dup2(STDOUT_FILENO, pipes[i][1]);
			}
			// dup other pipes
			// close remaining pipes
				j = 0;
				while (j < num_of_p + 1)
				{
					if (j != i)
						close(pipes[j][0]);
					if (j + 1 != i)
						close(pipes[j][1]);
				}
			nthcmd = get_nth_command(cmd, i);
			if (!nthcmd)
			{
				close(pipes[i][0]);
				close(pipes[i + 1][1]);
				printf(" error retrieving command\n");
				return (ERROR);
			}
			// INSTEAD OF ONLY EXECVE FIRST CHECK ALSO FOR BUILTINS AND OTHER COMMANDS EG ENV WITH RELATIVE PATH OR EXECUTABLES THAT ARE ALWAYS ON RELATIVE PATH
			if (execve(nthcmd->cmd, nthcmd->args, mini->env) == -1)
			{
				close(pipes[i][0]);
				close(pipes[i + 1][1]);
				printf(" error executing command\n");
				return (ERROR);
			}
		}
	}
	// close in and outfiles
	close(infile);
	close(outfile);
	// close all pipes
	i = 0;
	while (i < num_of_p + 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	// parent waits for all children
	i = 0;
	while (i < num_of_p)
	{
		wait(NULL);
	}





	if (pid == 0)
	{
		if (infile > -1)
		{
			dup2(infile, STDIN_FILENO);
			close(infile);
		}
		if (outfile > -1)
		{
			dup2(outfile, STDOUT_FILENO);
			close(outfile);
		}
		path = get_path_env(cmd->cmd);
		if (execve(path, cmd->args, mini->env) == -1)
			printf("could not execute\n");
	}
	wait(NULL);
	return (0);
}
