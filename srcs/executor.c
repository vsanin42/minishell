/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/21 20:05:05 by zpiarova         ###   ########.fr       */
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
/* int	executorr(t_mini *mini, t_cmd *cmd)
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
} */

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

	printf("num of p: %d\n", num_of_p);
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
			printf("forked process %d\n", i);
			nthcmd = get_nth_command(cmd, i);
			if (!nthcmd)
			{
				// close pipes
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
			// close remaining pipes
			j = 0;
			// while (j < num_of_p - 1)
			// {
			// 	if (j != i)
			// 	{
			// 		if (pipes[j][1] > STDIN_FILENO)
			// 			close(pipes[j][1]);
			// 	}
			// 	if (j!= i - 1)
			// 	{
			// 		if (pipes[j][0] > STDOUT_FILENO)
			// 			close(pipes[j][0]);
			// 	}
			// 	j++;
			// }
			// this closes all pipes since they are already dupped and we do not need them
			for (j = 0; j < num_of_p - 1; j++) {
				if (j != i) {
					close(pipes[j][1]); // Close write end of unused pipes
				}
				if (j != i - 1) {
					close(pipes[j][0]); // Close read end of unused pipes
				}
			}
			path = get_path_env(nthcmd->cmd);
			if (!path)
			{
				perror("minishell: command not found: ");
				perror(nthcmd->cmd);
				perror("\n");
				return (ERROR);
			}
			// INSTEAD OF ONLY EXECVE FIRST CHECK ALSO FOR BUILTINS AND OTHER COMMANDS EG ENV WITH RELATIVE PATH OR EXECUTABLES THAT ARE ALWAYS ON RELATIVE PATH
			if (execve(path, nthcmd->args, mini->env) == -1)
			{
				perror("minishell: error executing command: ");
				perror(nthcmd->cmd);
				perror("\n");
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
	for (i = 0; i < num_of_p; i++) {
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status)) {
			printf("Process %d exited with status %d\n", pids[i], WEXITSTATUS(status));
		} else {
			printf("Process %d terminated abnormally\n", pids[i]);
		}
	}
	printf("executed all\n");
	// close all pipes
	return (0);
}
