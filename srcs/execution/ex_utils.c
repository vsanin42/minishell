/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:02:34 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/17 13:04:32 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks if command is one of builtins and executes it
// will run only if is_builtin condition is true
// cannot call exit because it can be called also in main process
// returns 0 if any builtin was executed succesfully, and errno if not
int	exec_builtins(t_mini *mini, t_cmd *cmd)
{
	int	result;

	result = 0;
	if (!ft_strncmp(cmd->cmd, "cd", 2))
		result = cd_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "pwd", 3))
		result = pwd_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "env", 3))
		result = env_builtin(mini, cmd, NULL);
	else if (!ft_strncmp(cmd->cmd, "export", 6))
		result = export_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "unset", 5))
		result = unset_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "echo", 4))
		result = echo_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "exit", 4))
		exit_builtin(mini, cmd);
	else
		return (ERROR);
	return (result);
}

// receives a relative or absolute path and checks if it is an executable
// if is_executable_file > 0 means it is executable file and we found it
// if is_executable_file => 0 execve sets correct errno
int	exec_command_by_path(t_mini *mini, t_cmd *cmd)
{
	char	*path;

	path = cmd->cmd;
	if (execve(path, cmd->args, mini->env) == -1)
	{
		mini_perror(mini, create_msg("minishell", path, NULL, NULL));
		if (errno == 13)
			return (126);
	}
	return (ERROR);
}

// checks if command is shell command (=command at $PATH variable)
// @returns exit code if could not execute, nothing if executed command
// exits with 0 or exit code collected in main process
int	exec_shell_command(t_mini *mini, t_cmd *cmd)
{
	char	*path;

	path = get_path_env(mini, cmd->cmd);
	if (!path)
		return (mini_error(mini, create_msg("minishell",
					cmd->cmd, "command not found", NULL), 127));
	if (execve(path, cmd->args, mini->env) == -1)
		return (mini_perror(mini, create_msg("minishell",
					cmd->cmd, NULL, NULL)));
	return (ERROR);
}

// called at end of parent process to get exit status of ts last child
// @returns exit status of the last child
int	get_exit_status(int num_of_p, t_mini *mini, int *pids)
{
	int	i;
	int	status;
	int	exit_status;

	i = 0;
	status = 0;
	(void)mini;
	while (i < num_of_p)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_status = WTERMSIG(status) + 128;
		i++;
	}
	if (WTERMSIG(status) == SIGQUIT)
		write(2, "Quit\n", 5);
	return (exit_status);
}

int	init_int_arrs(t_mini *mini, int num_of_p)
{
	int	i;

	i = 0;
	mini->pids = (int *)malloc(sizeof(int) * num_of_p);
	if (!mini->pids)
		return (ERROR);
	if (num_of_p > 1)
	{
		mini->pipes = (int **)malloc(sizeof(int *) * (num_of_p));
		if (!mini->pipes)
			return (ERROR);
		while (i < num_of_p - 1)
		{
			mini->pipes[i] = (int *)malloc(sizeof(int) * 2);
			if (!mini->pipes[i])
				return (ERROR);
			i++;
		}
		mini->pipes[i] = NULL;
	}
	return (0);
}
