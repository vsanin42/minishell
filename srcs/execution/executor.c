/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/01 18:05:18 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks if command is one of builtins and executes it
// will run only if is_builtin condition is true
// cannot call exit because it can be called also in main process
// returns 0 if any builtin was found and executed successfully, 1 if not
int exec_builtins(t_mini *mini, t_cmd *cmd)
{
	int	result;

	result = 8;
	if (!ft_strncmp(cmd->cmd, "cd", 2))
		result = cd_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "pwd", 3))
		result = pwd_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "env", 3))
		result = env_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "export", 6))
		result = export_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "unset", 5))
		result = unset_builtin(mini, cmd);
	// else if (!ft_strncmp(cmd->cmd, "echo", 4))
	// 	result = echo_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "exit", 4))
		exit_builtin(mini);
	if (result > 0 && mini->error_msg)
		printf("%s\n", mini->error_msg);
	free(mini->error_msg);
	mini->error_msg = NULL;
	return (result);
}

// checks if command is specified by relative or absolute path
//  if is_executable > 0 means it is executable file and we found it
// if is_executable = 0 means we found it but it doesnt have execute permisions
// if we get to end, is_executable_file returned < 0 meaning file was not found
int exec_command_by_path(t_mini *mini, t_cmd *cmd)
{
	char	*path;
	int		is_executable;

	path = cmd->cmd;
	is_executable = is_executable_file(path);
	if (is_executable == 0)
		return (set_executor_error_msg(mini, path, "Permission denied", NULL), 9);
	else if (is_executable > 0)
	{
		if (execve(path, cmd->args, mini->env) == -1)
			return (set_executor_error_msg(mini, path, "Exec format error", NULL), 10);
	}
	return (set_executor_error_msg(mini, path, "No such file or directory", NULL), 11);
}

// checks if command is shell command (=command at $PATH variable)
int	exec_shell_command(t_mini *mini, t_cmd *cmd)
{
	char	 *path;

	path = get_path_env(mini, cmd->cmd);
	if (!path)
		return (set_executor_error_msg(mini, cmd->cmd, "command not found", NULL), 12);
	if (execve(path, cmd->args, mini->env) == -1)
		return (set_executor_error_msg(mini, path, "Exec format error", NULL), 13);
	return (14);
}

// we have to understand we call it command but it can also be path
// it actually always is path to the executable file -> command
// 1. first check builtin functions - do function for this later
// 2. check for executables starting with path - eg. ./minishell, ../minishell, minishell/minishell ... - it is already on path containing / - absolute or relative
// 3. means that only place left to look for are the commands at $PATH
int	execute(t_mini *mini, t_cmd *cmd)
{
	int	result;

	if (is_builtin(mini))
		result = exec_builtins(mini, mini->cmd_list);
	else if (ft_strchr(cmd->cmd, '/'))
		result = exec_command_by_path(mini, cmd);
	else
		result = exec_shell_command(mini, cmd);
	free_cmd_list(mini);
	//free_arr(mini->env);
	if (result != 0 && mini->error_msg)
		printf("%s\n", mini->error_msg);
	free(mini->error_msg);
	mini->error_msg = NULL;
	printf("result1: %d\n", result);
	return (result);
}

// check if builtin & 1 command - not open any processes, must be done in main
// create as many processes as commands - BUT each fork duplicates the existing process so we end up in more, thats why we are always using only the child process by: if pids[0] == 0
// and this child process is then killed by execve command at its end so we eventually get back to parent, he again creates only one child, and again ...
// for each process set the infile and outfile to the ones from its redir struct, if there is any
// set STDIN and STDOUT of each process to corresponding pipe or file
// close pipes and files - the ones we need are dupped anyways so we do not need them anymore
// execute - if the process continues, mans if failed or it was a builtin
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
			printf("result2: %d\n", mini->exit_status);
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
		printf("exit status: %d\n", mini->exit_status );
		i++;
	}
	printf("final exit status: %d\n", mini->exit_status );
	return (0);
}