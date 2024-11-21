/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/21 22:02:55 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	int		infile = STDIN_FILENO;
	int		outfile = STDOUT_FILENO;
	char	*path;
	t_redir	*redir;
	int		num_of_p = get_cmd_count(cmd);
	int		pids[num_of_p];
	int		pipes[num_of_p - 1][2];
	int		i;
	int		j;
	t_cmd *nthcmd;

	// open pipes between each process
	i = 0;
	while (i < num_of_p - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			printf("error creating pipes");
			while (i > 0)
			{
				close(pipes[i][1]);
				close(pipes[i][0]);
				i--;
			}
			return (ERROR);
		}
		i++;
	}
	// create as many processes as commands - BUT each fork duplicates the existing process so we end up in more, thats why we are always using only the child process by: if pids[0] == 0
	// and this child process is then killed by execve command at its end so we eventually get back to parent, he again creates only one child, and again ...
	i = 0;
	while (i < num_of_p)
	{
		// maybe here before forking add statement that if pid[i - 1] status code is 0 we can execute ths, set status code of pid[0] to 0
		pids[i] = fork();
		if (pids[i] == -1)
		{
			// close pipes and files if cannot fork
			// BUT have to close also all pipes and files in other processes???? even the ones that didnt even run ?
			i = 0;
			while (i < num_of_p - 1)
			{
				close(pipes[i][1]);
				close(pipes[i][0]);
				i++;
			}
			printf("error forking processes\n");
			return (ERROR);
		}
		// do this for every emerged child process
		if (pids[i] == 0) // do this in each new child
		{
			nthcmd = get_nth_command(cmd, i);
			if (!nthcmd)
			{
				i = 0;
				while (i < num_of_p - 1)
				{
					close(pipes[i][1]);
					close(pipes[i][0]);
					i++;
				}
				printf("error retrieving command\n");
				return (ERROR);
			}
			// for each process set the infile and outfile to the ones from its redir
			redir = nthcmd->redir;
			while (redir)
			{
				if (redir->type == TOKEN_REDIRIN)
				{
					if (infile > STDIN_FILENO)
						close(infile);
					infile = open(redir->file, O_RDONLY);
				}
				else if (redir->type == TOKEN_REDIROUT)
				{
					if (outfile > STDOUT_FILENO)
						close(outfile);
					outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				}
				else if (redir->type == TOKEN_APPEND)
				{
					if (outfile > STDOUT_FILENO)
						close(outfile);
					outfile = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				}
				redir = redir->next;
			}
			// set up first process
			if (i == 0)
			{
				dup2(infile, STDIN_FILENO);
				if (num_of_p - 1 > 0) // so if we have no pipe open when only 1 process we dont use pipe WR end - not working, still writes to pipe
					dup2(pipes[0][1], STDOUT_FILENO);
			}
			// set ends op pipes to middle processes
			if (i > 0 && i < num_of_p - 1)
			{
				dup2(pipes[i - 1][0], STDIN_FILENO);
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			// set up outfile - read end of last pipe
			if (i == num_of_p - 1)
			{
				if (num_of_p - 1 > 0) // so if we have no pipe open when only 1 process we dont use pipe RD end - not working, still reads from pipe
					dup2(pipes[i - 1][0], STDIN_FILENO);
				dup2(outfile, STDOUT_FILENO);
			}
			// close in and outfiles - they are dupped so we do not need them anymore
			if (infile > STDIN_FILENO)
				close(infile);
			if (outfile > STDOUT_FILENO)
				close(outfile);
			// close pipes - the ones we need are dupped anyways so we do not ned them anymore
			j = 0;
			while (j < num_of_p - 1)
			{
				close(pipes[j][1]); // Close write end of unused pipes
				close(pipes[j][0]); // Close read end of unused pipes
				j++;
			}
			// executing builtin functions - do function for this later
			if (!ft_strncmp(nthcmd->cmd, "cd", 2))
			{
				printf("executing cd builtin\n");
				if (nthcmd->args[2])
					return (ERROR);
				if (cd_builtin(nthcmd->args[1]) == ERROR)
					return (ERROR);
				return (0);
			}
			// check if the command exists - eg. lkasjkl as command cannot work
			if (access(nthcmd->cmd, F_OK) == 0)
			{
				perror("minishell: command not found: ");
				perror(nthcmd->cmd);
				return (ERROR);
			}
			// check for executables starting with path - eg. ./minishell, ../minishell, minishell/minishell ... - it is already on path

			// INSTEAD OF ONLY EXECVE FIRST CHECK ALSO FOR BUILTINS AND OTHER COMMANDS EG ENV WITH RELATIVE PATH OR EXECUTABLES THAT ARE ALWAYS ON RELATIVE PATH
			// check for shell commands or commands in path
			path = get_path_env(nthcmd->cmd);
			if (!path)
			{
				perror("minishell: command not found: ");
				perror(nthcmd->cmd);
				perror("\n");
				return (ERROR);
			}
			if (execve(path, nthcmd->args, mini->env) == -1)
			{
				// perror("minishell: error executing command: ");
				// perror(nthcmd->cmd);
				// perror("\n");
				return (ERROR);
			}
		}
		i++;
	}
	// close all pipes
	i = 0;
	while (i < num_of_p - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	// parent waits for all children
	// i = -1;
	// while (++i < num_of_p)
	// 	wait(NULL);
	int status;
	i = 0;
	while (i < num_of_p)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			printf("Process %d exited with status %d\n", pids[i], WEXITSTATUS(status));
		else
			printf("Process %d terminated abnormally\n", pids[i]);
		i++;
	}
	printf("executed all\n");
	return (0);
}
