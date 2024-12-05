/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/05 18:20:01 by zpiarova         ###   ########.fr       */
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
		mini->exit_status = unset_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "echo", 4))
		mini->exit_status = echo_builtin(mini, cmd);
	else if (!ft_strncmp(cmd->cmd, "exit", 4))
		exit_builtin(mini);
	else // return error if no builtin matched - need to free error msg???
		return (ERROR); // mini->exit_status = 0;
	if (mini->exit_status != 0 && mini->error_msg)
		printf("%s\n", mini->error_msg);
	free(mini->error_msg);
	mini->error_msg = NULL;
	return (0); // no need to return exit status
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
	//write(1, "here1", 5);
	if (is_executable == 0)
	{
		perror_msg(path);
		write(1, "here2", 5);
		printf("errno2: %d\n", errno);
		return (ERROR);
	}
	//return (set_executor_error_msg(mini, path, "Permission denied", NULL), 9);
	else if (is_executable > 0)
	{
		if (execve(path, cmd->args, mini->env) == -1)
		{
			//write(1, "here3", 5);
			perror_msg(path);
			return (ERROR);
		}
		return (0);
		//return (set_executor_error_msg(mini, path, "Exec format error", NULL), 10);
	}
	else
	{
		//write(1, "here4", 5);
		perror_msg(path);
		return (ERROR);
	}
}

// checks if command is shell command (=command at $PATH variable)
int	exec_shell_command(t_mini *mini, t_cmd *cmd)
{
	char	*path;

	path = get_path_env(mini, cmd->cmd);
	if (!path)
		return (ERROR);
		//return (set_executor_error_msg(mini, cmd->cmd, "command not found", NULL), 12);
	if (execve(path, cmd->args, mini->env) == -1)
	{
		printf("here ");
		perror_msg(path);
		return (ERROR);
	}
		//return (set_executor_error_msg(mini, path, "Exec format error", NULL), 13);
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
	mini->exit_status = result;
	exit(mini->exit_status);
}

int perform_child(t_mini *mini, int i, int num_of_p, int pipes[][2])
{
	t_cmd	*nthcmd;
	int		files[2];

	files[0] = STDIN_FILENO;
	files[1] = STDOUT_FILENO;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	set_termios(0);
	nthcmd = get_nth_command(mini->cmd_list, i);
	if (!nthcmd)
	{
		close_all_pipes(pipes, num_of_p);
		return (ERROR);
	}
	set_files(nthcmd, &files[0], &files[1]);
	set_ins_outs(i, pipes, files, num_of_p);
	close_files(&files[0], &files[1]);
	close_all_pipes(pipes, num_of_p);
	if (nthcmd->cmd)
		mini->exit_status = execute(mini, nthcmd);
	return (0);
}

// check if builtin & 1 command - not open any processes, must be done in main
// create as many processes as commands - BUT each fork duplicates the existing process so we end up in more, thats why we are always using only the child process by: if pids[0] == 0
// and this child process is then killed by execve command at its end so we eventually get back to parent, he again creates only one child, and again ...
// for each process set the infile and outfile to the ones from its redir struct, if there is any
// set STDIN and STDOUT of each process to corresponding pipe or file
// close pipes and files - the ones we need are dupped anyways so we do not need them anymore
// execute - if the process continues, mans if failed or it was a builtin
// BEFORE FORK:
// 	ERROR - PERROR AND RETURNS ERROR TO CALLER FUNCTION PROCESS_INPUT
// 	SUCCESS - CONTINUES TO CHILD CREATION OR RETURNS 0 TO CALLER IN CASE OF SINGLE BUILTIN
// AFTER FORK = IN CHILD PROCESSES:
// 	SUCCESS:
// 		- FOR BUILTIN: RETURNS 0 TO CALLER
// 		- FOR OTHERS: EXECUTION HAPPENS, NOTHING TO BE DONE
// 	ERROR: ALWAYS FREE && EACH CHILD MUST EXIT OR END IN EXECVE
// 		- FOR BUILTIN: PERROR, FREE ALL AND RETURN 1 TO CALLER,
// 		- FOR OTHERS: PERROR, FREE ALL AND RETURN 1 TO CALLER
int	executor(t_mini *mini)
{
	int		num_of_p = get_cmd_count(mini->cmd_list);
	int		pids[num_of_p];
	int		pipes[num_of_p - 1][2];
	int		i;

	if (validate_files(mini) == 1)
		return (ERROR);
	if (is_builtin(mini))
		return (exec_builtins(mini, mini->cmd_list));
	if (open_pipes(pipes, num_of_p) == ERROR)
		return (ERROR);
	signal(SIGINT, sigint_void);
	i = 0;
	while (i < num_of_p)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("minishell");
			close_all_pipes(pipes, num_of_p);
			return (ERROR);
		}
		if (pids[i] == 0)
		{
			perform_child(mini, i, num_of_p, pipes);
		}
		i++;
	}
	close_all_pipes(pipes, num_of_p);
	set_exit_status(num_of_p, mini, pids);
	//printf("\nstatus: %d\n", mini->exit_status);
	return (0);
}

void	set_exit_status(int num_of_p, t_mini *mini, int *pids)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < num_of_p)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			mini->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			mini->exit_status = WTERMSIG(status) + 128;
			if (WTERMSIG(status) == SIGQUIT)
				printf("Quit: 3\n");
		}
		i++;
	}
}
