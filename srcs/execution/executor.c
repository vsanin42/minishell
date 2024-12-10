/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/10 15:25:11 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks if command is one of builtins and executes it
// will run only if is_builtin condition is true
// cannot call exit because it can be called also in main process
// returns 0 if any builtin was excuted succesfully, and errno if not
int exec_builtins(t_mini *mini, t_cmd *cmd)
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
		exit_builtin(mini);
	else
		return (ERROR);
	return (result);
}

int exec_builtin_in_parent(t_mini *mini, int files[2])
{
	int	result;
	int	stdin;			// this can probably go away since it is done in main function
	int	stdout;			// this can probably go away since it is done in main function
	set_files(mini, mini->cmd_list, &files[0], &files[1]);
	stdin = dup(STDIN_FILENO);
	stdout = dup(STDOUT_FILENO);
	dup2(files[0], STDIN_FILENO);
	dup2(files[1], STDOUT_FILENO);
	close_files(&files[0], &files[1]);
	result = exec_builtins(mini, mini->cmd_list);
	dup2(stdin, 0);		// this can probably go away since it is done in main function
	dup2(stdout, 1);	// this can probably go away since it is done in main function
	mini->exit_status = result;
	return (result);
}

// checks if command is specified by relative or absolute path
//  f is_executable_file > 0 means it is executable file and we found it
// if is_executable_file => 0 execve sets correct errno
int exec_command_by_path(t_mini *mini, t_cmd *cmd)
{
	char	*path;
	int		result;

	result = 0;
	path = cmd->cmd;
	if (is_executable_file(path) == -1)
	{
		perror("minishell");
		result = errno;
		return (result);
	}
	if (execve(path, cmd->args, mini->env) == -1)
	{
		result = errno;
		perror("minishell");
		return (result);
	}
	return (result);
}

// checks if command is shell command (=command at $PATH variable)
int	exec_shell_command(t_mini *mini, t_cmd *cmd)
{
	char	 *path;
	int		result;

	result = 0;
	path = get_path_env(mini, cmd->cmd);
	if (!path)
		return (mini_error(mini, cmd->cmd, "command not found", NULL), 127);
	if (execve(path, cmd->args, mini->env) == -1)
	{
		result = errno;
		perror("minishell");
	}
	return (result);
}

// we have to understand we call it command but it can also be path
// it actually always is path to the executable file -> command
// 1. first check builtin functions - do function for this later
// 2. check for executables starting with path - eg. ./minishell, ../minishell, minishell/minishell ... - it is already on path containing "/" - absolute or relative
// 3. means that only place left to look for are the commands at $PATH
int	execute(t_mini *mini, t_cmd *cmd)
{
	int	result;

	result = 0;
	if (is_builtin(cmd))
		result = exec_builtins(mini, cmd);
	else if (ft_strchr(cmd->cmd, '/'))
		result = exec_command_by_path(mini, cmd);
	else
		result = exec_shell_command(mini, cmd);
	free_cmd_list(mini);
	free_arr(mini->env);
	return (result);
}

// check if builtin & 1 command - not open any processes, must be done in main
// create as many processes as commands - BUT each fork duplicates the existing process so we end up in more, thats why we are always using only the child process by: if pids[0] == 0
// and this child process is then killed by execve command at its end so we eventually get back to parent, he again creates only one child, and again ...
// for each process set the infile and outfile to the ones from its redir struct, if there is any
// set STDIN and STDOUT of each process to corresponding pipe or file
// close pipes and files - the ones we need are dupped anyways so we do not need them anymore
// execute - if the process continues, means if failed or it was a builtin - we have to exit with proper error code
int	executor(t_mini *mini)
{
	int		files[2];
	int		num_of_p = get_cmd_count(mini->cmd_list);
	int		pids[num_of_p];
	int		pipes[num_of_p - 1][2];
	int		i;
	t_cmd	*nthcmd;
	int		result;

	result = 0;
	i = 0;
	files[0] = STDIN_FILENO;
	files[1] = STDOUT_FILENO;
	if (num_of_p == 1 && is_builtin(mini->cmd_list))
		return (exec_builtin_in_parent(mini, files));
	result = open_pipes(pipes, num_of_p);
	if (result != 0)
		return (result);
	signal(SIGINT, SIG_IGN);
	while (i < num_of_p)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			close_all_pipes(pipes, num_of_p);
			perror("minishell");
			return (ERROR);
		}
		if (pids[i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			set_termios(0);
			nthcmd = get_nth_command(mini->cmd_list, i);
			if (!nthcmd)
			{
				close_all_pipes(pipes, num_of_p);
				return (mini_error(mini, "command not found", NULL, NULL), 127);
			}
			set_files(mini, nthcmd, &files[0], &files[1]);
			set_ins_outs(i, pipes, files, num_of_p);
			close_files(&files[0], &files[1]);
			close_all_pipes(pipes, num_of_p);
			if (nthcmd->cmd)
				result = execute(mini, nthcmd);
			mini->exit_status = result;
			exit(result); // have to exit here in process, not in execute function, because that runs only if there is command - but what if we run process with only files? it would never exit bc. execute function that can exit would never run
		}
		i++;
	}
	close_all_pipes(pipes, num_of_p);
	set_exit_status(num_of_p, mini, pids);
	return (mini->exit_status);
}

void	ses_help(t_mini *mini, int *signaled, int *status, int *last_sig)
{
	if (WIFEXITED(*status))
		mini->exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		mini->exit_status = WTERMSIG(*status) + 128;
		*signaled = 1;
		*last_sig = WTERMSIG(*status);
	}
}

void	ses_init(int *signaled, int *i, int *status, int *last_sig)
{
	*signaled = 0;
	*i = 0;
	*status = 0;
	*last_sig = 0;
}

void	set_exit_status(int num_of_p, t_mini *mini, int *pids)
{
	int	i;
	int	status;
	int	signaled;
	int	last_sig;

	ses_init(&signaled, &i, &status, &last_sig);
	while (i < num_of_p)
	{
		waitpid(pids[i], &status, 0);
		ses_help(mini, &signaled, &status, &last_sig);
		i++;
	}
	if (signaled)
	{
		if (last_sig == SIGINT)
			write(1, "\n", 1);
		else if (last_sig == SIGQUIT)
			write(1, "Quit\n", 5);
	}
}
