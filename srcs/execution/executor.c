/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/29 13:39:56 by zpiarova         ###   ########.fr       */
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
		mini->exit_status = cd_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "pwd", 3))
		mini->exit_status = pwd_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "env", 3))
		mini->exit_status = env_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "export", 6))
		mini->exit_status = export_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "unset", 5))
		mini->exit_status = unset_builtin(mini, cmd);
	// else if (!ft_strncmp(cmd->cmd, "echo", 4))
	// 	mini->exit_status = echo_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "exit", 4))
		return (exit_builtin(mini), ERROR);
	if (mini->exit_status != 0 && mini->error_msg)
		printf("%s\n", mini->error_msg);
	free(mini->error_msg);
	mini->error_msg = NULL;
	return (mini->exit_status);
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
		|| !ft_strncmp(mini->cmd_list->cmd, "unset", 5) || !ft_strncmp(mini->cmd_list->cmd, "exit", 4)))
	{
		return (exec_builtins(mini, mini->cmd_list));
	}
	// 2. check for executables starting with path - eg. ./minishell, ../minishell, minishell/minishell ... - it is already on path
	if (ft_strchr(cmd->cmd, '/')) // if contains baskslash shell interprets it as a path to specific file - absolute or relative
	{
		if (exec_command_by_path(mini, cmd) == ERROR)
		{
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

// check if builtin & 1 comand - not open any processes, must be done in main
// create as many processes as commands - BUT each fork duplicates the existing process so we end up in more, thats why we are always using only the child process by: if pids[0] == 0
// and this child process is then killed by execve command at its end so we eventually get back to parent, he again creates only one child, and again ...
// for each process set the infile and outfile to the ones from its redir struct, if there is any
// set STDIN and STDOUT of each process to correcsponding pipe or file
// close pipes and files - the ones we need are dupped anyways so we do not need them anymore
// execute - if the process condinues, mans if failed or it was a builtin
int	executor(t_mini *mini)
{
	int		files[2];
	int		num_of_p = get_cmd_count(mini->cmd_list);
	int		pids[num_of_p + 1];
	int		pipes[num_of_p - 1][2];
	int		i;
	t_cmd	*nthcmd;

	files[0] = STDIN_FILENO;
	files[1] = STDOUT_FILENO;
	if (is_builtin(mini))
		return (exec_builtins(mini, mini->cmd_list));
	if (open_pipes(pipes, num_of_p) == ERROR)
		return (ERROR);
	i = 0;
	while (i < num_of_p)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			close_all_pipes(pipes, num_of_p);
			perror("error forking processes\n");
			return (ERROR);
		}
		if (pids[i] == 0)
		{
			nthcmd = get_nth_command(mini->cmd_list, i);
			if (!nthcmd)
			{
				close_all_pipes(pipes, num_of_p);
				perror("error retrieving command\n");
				return (ERROR);
			}
			set_files(nthcmd, &files[0], &files[1]);
			set_ins_outs(i, pipes, files, num_of_p);
			close_files(&files[0], &files[1]);
			close_all_pipes(pipes, num_of_p);
			if (nthcmd->cmd)
				mini->exit_status = execute(mini, nthcmd);
			free_cmd_list(mini);
			free_arr(mini->env);
			if (mini->exit_status != 0 && mini->error_msg)
				printf("%s\n", mini->error_msg);
			free(mini->error_msg);
			mini->error_msg = NULL;
			//return (mini->exit_status);
			exit(mini->exit_status);
		}
		i++;
	}
	pids[i] = '\0';
	close_all_pipes(pipes, num_of_p);
	i = 0;
	while (i < num_of_p)
	{
		waitpid(pids[i], &mini->exit_status, 0);
		mini->exit_status = WIFEXITED(mini->exit_status);
		i++;
	}
	return (0);
}
