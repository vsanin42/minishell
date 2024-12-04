/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/04 10:57:59 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
// A B B A

// checks if command is one of builtins and executes it
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
		exit_builtin(mini);
	else
		mini->exit_status = 0;
	if (mini->exit_status != 0 && mini->error_msg)
		printf("%s\n", mini->error_msg);
	free(mini->error_msg);
	mini->error_msg = NULL;
	return (mini->exit_status);
}

// checks if command is specified by relative or absolute path
//  if is_executable > 0 means it is executable file and we found it
// if is_executable > 0 means we found it but it doesnt have execute permisions
// if we get to end, is_executable_file returned < 0 meaning file was not found
int exec_command_by_path(t_mini *mini, t_cmd *cmd)
{
	char	*path;
	int		is_executable;

	path = cmd->cmd;
	is_executable = is_executable_file(path);
	if (is_executable > 0)
	{
		if (execve(path, cmd->args, mini->env) == -1)
			return (set_executor_error_msg(mini, path, "Exec format error", NULL), ERROR);
	}
	else if (is_executable == 0)
		return (set_executor_error_msg(mini, path, "Permission denied", NULL), 0);
	return (0);
}

// checks if command is shell command (=command at $PATH variable)
int	exec_shell_command(t_mini *mini, t_cmd *cmd)
{
	char	 *path;

	path = get_path_env(mini, cmd->cmd);
	if (!path)
		return (set_executor_error_msg(mini, cmd->cmd, "command not found", NULL), ERROR);
	if (execve(path, cmd->args, mini->env) == -1)
		return (set_executor_error_msg(mini, path, "Exec format error", NULL), ERROR);
	return (0);
}

int	execute(t_mini *mini, t_cmd *cmd)
{
	// we have to understand we call it command if it is the first text in cmd but it can also be path (it actually always is path to the executable file - command)
	// so we put errors: for builtins command not found, for checking path no such file or directory, for shell executables command not found
	// 1. first check builtin functions - do function for this later
	if (exec_builtins(mini, mini->cmd_list) != 0)
	{
		return (mini->exit_status);
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
