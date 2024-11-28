/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/28 17:23:14 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* ERROR MESSAGE */

void	set_executor_error_msg(t_mini *mini, char *first, char *second, char *third)
{
	char *msg;
	char *old_msg;
	char *new_part;

	free(mini->error_msg);
	mini->error_msg = NULL;
	msg = ft_strdup("minishell: ");
	if (first)
	{
		old_msg = msg;
		new_part = ft_strdup(first);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	if (second)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(second);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	if (third)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(third);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	mini->error_msg = msg;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* MANAGING FILES AND PIPES */

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
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* EXECUTION */

// 1. case - builtins
// returns 0 if any builtin was excuted succesfully, 1 if not
int exec_builtins(t_mini *mini, t_cmd *cmd)
{
	if (!ft_strncmp(cmd->cmd, "cd", 2))
	{
		// if there is more than 1 argument return error
		if (cd_builtin(mini, cmd) == 0)
		{
			free(mini->error_msg);
			mini->error_msg = NULL;
			return (0); // return true because we did the action and can exit
		}
		return (ERROR); // error because we found the command, there was just error while executing it
	}
	else if (!ft_strncmp(cmd->cmd, "pwd", 3))
	{
		if (pwd_builtin(mini, cmd) == 0)
		{
			free(mini->error_msg);
			mini->error_msg = NULL;
			return (0); // return true becasue we did the action and can exit
		}
		return (ERROR); // error because we found the command, there was just error while executing it
	}
	else if (!ft_strncmp(cmd->cmd, "env", 3))
	{
		if (env_builtin(mini, cmd) == 0)
		{
			free(mini->error_msg);
			mini->error_msg = NULL;
			return (0); // return true becasue we did the action and can exit
		}
		return (ERROR); // error because we found the command, there was just error while executing it
	}
	else if (!ft_strncmp(cmd->cmd, "export", 6))
	{
		if (export_builtin(mini, cmd) == 0)
		{
			free(mini->error_msg);
			mini->error_msg = NULL;
			return (0); // return true becasue we did the action and can exit
		}
		return (ERROR); // error because we found the command, there was just error while executing it
	}
	else if (!ft_strncmp(cmd->cmd, "unset", 5))
	{
		if (unset_builtin(mini, cmd) == 0)
		{
			free(mini->error_msg);
			mini->error_msg = NULL;
			return (0); // return true becasue we did the action and can exit
		}
		return (ERROR); // error because we found the command, there was just error while executing it
	}
	// else if (!ft_strncmp(cmd->cmd, "echo", 4))
	// {
	// 	if (echo_builtin(mini, cmd) == 0)
	// 	{
	// 		free(mini->error_msg);
	// 		mini->error_msg = NULL;
	// 		return (0); // return true becasue we did the action and can exit
	// 	}
	// 	return (ERROR); // error because we found the command, there was just error while executing it
	// }
	else
		return (ERROR);
}

// 2. case - command specified by relative or absolute path
int exec_command_by_path(t_mini *mini, t_cmd *cmd)
{
	char *path;

	path = cmd->cmd;
	if (is_executable_file(path) > 0) // is ececutable file and we found it
	{
		if (execve(path, cmd->args, mini->env) == -1)
		{
			set_executor_error_msg(mini, path, "Exec format error", NULL);
			return (ERROR); // ERROR because we found it and it was executable path but something went wrong when executing
		}
	}
	else if (!is_executable_file(path))
	{
		set_executor_error_msg(mini, path, "Permission denied", NULL);
		return (2); /// CONTINUE because it it not executable file on this path but can be shell command
	}
	// if we get here is_executable_file returned < 0 which means it was not found
	set_executor_error_msg(mini, path, "No such file or directory", NULL);
	return (2); // CONTINUE because it it not found by is_executable file but it can be shell command
}

// 3. case - shell commands OR commands at $PATH variable
int exec_shell_command(t_mini *mini, t_cmd *cmd)
{
	char	 *path;

	path = get_path_env(mini, cmd->cmd);
	if (!path)
	{
		set_executor_error_msg(mini, cmd->cmd, "command not found", NULL);
		return (ERROR);
	}
	if (execve(path, cmd->args, mini->env) == -1)
	{
		set_executor_error_msg(mini, path, "Exec format error", NULL);
		return (ERROR);
	}
	return (ERROR);
}


int	execute(t_mini *mini, t_cmd *cmd)
{
	// we have to understand we call it command if it is the first text in cmd but it can also be path (it actually always is path to the executable file - command)
	// so we put errors: for builtins command not found, for checking path no such file or directory, for shell executables command not found
	// 1. first check builtin functions - do function for this later
	if (mini->cmd_list && mini->cmd_list->cmd && (!ft_strncmp(mini->cmd_list->cmd, "cd", 2) || !ft_strncmp(mini->cmd_list->cmd, "pwd", 3) || !ft_strncmp(mini->cmd_list->cmd, "env", 3) || !ft_strncmp(mini->cmd_list->cmd, "export", 6)
		|| !ft_strncmp(mini->cmd_list->cmd, "unset", 5)))
	{
		return (exec_builtins(mini, mini->cmd_list));
	}
	// 2. check for executables starting with path - eg. ./minishell, ../minishell, minishell/minishell ... - it is already on path
	if (ft_strchr(cmd->cmd, '/')) // if contains baskslash shell interprets it as a path to specific file - absolute or relative
	{
		printf("executing here\n");
		if (exec_command_by_path(mini, cmd) == ERROR)
		{
			printf("execution had error here\n");
			return (ERROR);
		}
	}
	// 3. means that only place left to look for are the shell commands
	else
	{
		if (exec_shell_command(mini, cmd) == ERROR)
		{
			return (ERROR);
		}
	}
	return (ERROR);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* CALLING EXECUTOR FUNCTION */

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

	if (mini->cmd_list && !mini->cmd_list->next && mini->cmd_list->cmd && (!ft_strncmp(mini->cmd_list->cmd, "cd", 2) || !ft_strncmp(mini->cmd_list->cmd, "pwd", 3) || !ft_strncmp(mini->cmd_list->cmd, "env", 3) || !ft_strncmp(mini->cmd_list->cmd, "export", 6)
		|| !ft_strncmp(mini->cmd_list->cmd, "unset", 5)))
	{
		i = exec_builtins(mini, mini->cmd_list);
		if (i == ERROR)
		{
			printf("%s\n", mini->error_msg);
			free(mini->error_msg);
			mini->error_msg = NULL;
			return (ERROR);
		}
		free(mini->error_msg);
		mini->error_msg = NULL;
		return (0);
	}
	// open pipes between each process
	if (open_pipes(pipes, num_of_p) == ERROR)
		return (ERROR);
	i = 0;
	while (i < num_of_p)
	{
		// maybe here before forking add statement that if pid[i - 1] status code is 0 we can execute this, set status code of pid[0] to 0
		pids[i] = fork();
		if (pids[i] == -1)
		{
			close_all_pipes(pipes, num_of_p);
			perror("error forking processes\n");
			return (ERROR);
		}
		// do this for every emerged child process
		if (pids[i] == 0)
		{
			nthcmd = get_nth_command(mini->cmd_list, i);
			if (!nthcmd)
			{
				close_all_pipes(pipes, num_of_p);
				perror("error retrieving command\n");
				return (ERROR);
			}
			// for each process set the infile and outfile to the ones from its redir struct, if there is any
			set_files(nthcmd, &infile, &outfile);
			// set up first process
			set_ins_outs(i, pipes, &infile, &outfile, num_of_p);
			// close pipes and files - the ones we need are dupped anyways so we do not need them anymore
			close_files(&infile, &outfile);
			close_all_pipes(pipes, num_of_p);
			int result = -1;
			if (nthcmd->cmd)
			{
				result = execute(mini, nthcmd);
				// must free all in this process !!!
			}
			free_cmd_list(mini->cmd_list);
			free_arr(mini->env);	
			if (result == 0)
			{
				free(mini->error_msg);
				mini->error_msg = NULL;
				exit(0); // or other status code
			}
			else
			{
				if (mini->error_msg)
					printf("%s\n", mini->error_msg);
				free(mini->error_msg);
				mini->error_msg = NULL;
				exit(ERROR);
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
		// if (WIFEXITED(status))
		// 	printf("Process %d exited with status %d\n", pids[i], WEXITSTATUS(status)); // then we will set status of our program
		// else
		// 	printf("Process %d terminated abnormally\n", pids[i]); // then we will set status of our program
		i++;
	}
	return (0);
}