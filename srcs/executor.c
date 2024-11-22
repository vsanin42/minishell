/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/22 11:58:14 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	close_files(int *infile, int *outfile)
{
	if (*infile > STDIN_FILENO)
		close(*infile);
	if (*outfile > STDOUT_FILENO)
		close(*outfile);
	// can closing go wrong?
	return (0);
}

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

int	open_pipes(int pipes[][2], int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count - 1)
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

int	set_ins_outs(int i, int pipes[][2], int *infile, int *outfile, int num_of_p)
{
	if (i == 0)
	{
		dup2(*infile, STDIN_FILENO);
		if (num_of_p - 1 > 0) // so if we have no pipe open when only 1 process we dont use pipe WR end - not working, still writes to pipe
		{
			if (*outfile > STDOUT_FILENO)
				dup2(*outfile, STDOUT_FILENO);
			else
				dup2(pipes[0][1], STDOUT_FILENO);
		}
	}
	// set ends of pipes to middle processes
	if (i > 0 && i < num_of_p - 1)
	{
		if (*infile > STDIN_FILENO)
			dup2(*infile, STDIN_FILENO);
		else
			dup2(pipes[i - 1][0], STDIN_FILENO);
		if (*outfile > STDOUT_FILENO)
			dup2(*outfile, STDOUT_FILENO);
		else
			dup2(pipes[i][1], STDOUT_FILENO);
	}
	// set up outfile - read end of last pipe
	if (i == num_of_p - 1)
	{
		if (num_of_p - 1 > 0) // so if we have no pipe open when only 1 process we dont use pipe RD end - not working, still reads from pipe
		{
			if (*infile > STDIN_FILENO)
				dup2(*infile, STDIN_FILENO);
			else
				dup2(pipes[i - 1][0], STDIN_FILENO);
		}
		dup2(*outfile, STDOUT_FILENO);
	}
	return (0);
}

int	set_files(t_cmd *nthcmd, int *infile, int *outfile)
{
	t_redir	*redir;

	redir = nthcmd->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRIN)
		{
			if (*infile > STDIN_FILENO)
				close(*infile);
			*infile = open(redir->file, O_RDONLY);
			// it can go wrong? maybe set errorcode and return it
		}
		else if (redir->type == TOKEN_REDIROUT)
		{
			if (*outfile > STDOUT_FILENO)
				close(*outfile);
			*outfile = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			// it can go wrong? maybe set errorcode and return it
		}
		else if (redir->type == TOKEN_APPEND)
		{
			if (*outfile > STDOUT_FILENO)
				close(*outfile);
			*outfile = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			// it can go wrong? maybe set errorcode and return it
		}
		redir = redir->next;
	}
	return (0);
}

int	check_builtins(t_cmd *cmd)
{
	printf("checking builtins\n");
	if (!ft_strncmp(cmd->cmd, "cd", 2))
	{
		printf("inside cd\n");
		printf("args[1]: %s\n", cmd->args[1]);
		// if there is more than 1 argument return error
		if (get_args_len(cmd)  > 2)
			return (ERROR);
		if (cd_builtin(cmd->args[1]) == ERROR)
			return (ERROR);
	}
	else if (!ft_strncmp(cmd->cmd, "pwd", 3))
	{
		if (pwd_builtin() == ERROR)
			return (ERROR);
	}
	return (0);
}

int	execute(t_mini *mini, t_cmd *cmd)
{
	// we have to understand we call it command if it is the first text in cmd but it can also be path (it actually always is path)
	// so we put errors: for builtins command not found, for checking on opath no such file or directory, for shell executables command not found
	char	*path;

	// 1. first check builtin functions - do function for this later
	if (check_builtins(cmd) == ERROR)
	{
		// but also return out if it is 0 becaue wewant ot exit this process, and builtins do not do it themselves as execve
		return (ERROR);
	}

	// 2. check for executables starting with path - eg. ./minishell, ../minishell, minishell/minishell ... - it is already on path
	// 3. means that only place left to look for are the shell commands
	// check if the command exists - eg. lkasjkl as command cannot work
	if (access(cmd->cmd, F_OK) == 0)
	{
		perror("minishell: command not found: ");
		perror(cmd->cmd);
		return (ERROR);
	}

	// INSTEAD OF ONLY EXECVE FIRST CHECK ALSO FOR BUILTINS AND OTHER COMMANDS EG ENV WITH RELATIVE PATH OR EXECUTABLES THAT ARE ALWAYS ON RELATIVE PATH
	// check for shell commands or commands in path

	path = get_path_env(cmd->cmd);
	if (!path)
	{
		perror("minishell: command not found: ");
		perror(cmd->cmd);
		perror("\n");
		return (ERROR);
	}
	if (execve(path, cmd->args, mini->env) == -1)
	{
		perror("minishell: error executing command: ");
		perror(cmd->cmd);
		perror("\n");
		return (ERROR);
	}
	return (ERROR);
}

// create as many processes as commands - BUT each fork duplicates the existing process so we end up in more, thats why we are always using only the child process by: if pids[0] == 0
// and this child process is then killed by execve command at its end so we eventually get back to parent, he again creates only one child, and again ...
int	executor(t_mini *mini)
{
	int		infile = STDIN_FILENO;
	int		outfile = STDOUT_FILENO;
	int		num_of_p = get_cmd_count(mini->cmd_list);
	int		pids[num_of_p + 1];
	int		pipes[num_of_p - 1][2];
	int		i;
	t_cmd	*nthcmd;

	// open pipes between each process
	if (open_pipes(pipes, num_of_p) == ERROR)
	{
		return (ERROR);
	}
	i = 0;
	while (i < num_of_p)
	{
		// maybe here before forking add statement that if pid[i - 1] status code is 0 we can execute this, set status code of pid[0] to 0
		pids[i] = fork();
		if (pids[i] == -1)
		{
			close_all_pipes(pipes, num_of_p);
			printf("error forking processes\n");
			return (ERROR);
		}
		// do this for every emerged child process
		if (pids[i] == 0)
		{
			nthcmd = get_nth_command(mini->cmd_list, i);
			if (!nthcmd)
			{
				close_all_pipes(pipes, num_of_p);
				printf("error retrieving command\n");
				return (ERROR);
			}
			// for each process set the infile and outfile to the ones from its redir struct, if there is any
			set_files(nthcmd, &infile, &outfile);
			// set up first process
			set_ins_outs(i, pipes, &infile, &outfile, num_of_p);
			// close pipes and files - the ones we need are dupped anyways so we do not need them anymore
			close_files(&infile, &outfile);
			close_all_pipes(pipes, num_of_p);
			if (execute(mini, nthcmd) == 1)
			{
				// must free all in this process !!!
				free_cmd_list(mini->cmd_list);
				exit(1); // or other status code
			}
		}
		i++;
	}
	pids[i] = '\0';
	close_all_pipes(pipes, num_of_p);
	// parent waits for all children
	int status;
	i = 0;
	while (i < num_of_p)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			printf("Process %d exited with status %d\n", pids[i], WEXITSTATUS(status)); // then we will set status of our program
		else
			printf("Process %d terminated abnormally\n", pids[i]); // then we will set status of our program
		i++;
	}
	return (0);
}
