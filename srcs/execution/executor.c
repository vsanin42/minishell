/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:41:26 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/17 19:05:31 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_int_arr(int **pipes, int *pids)
{
	int	**head;

	if (!pipes && !pids)
		return ;
	if (pipes)
	{
		head = pipes;
		if (!*pipes)
			return ;
		while (*pipes)
		{
			free(*pipes);
			*pipes = NULL;
			pipes++;
		}
		free(head);
		head = NULL;
	}
	if (pids)
	{
		free(pids);
		pids = NULL;
	}
}

// if it is only one process and is builtin, execute it in parent process
// because it manipulates resources about process itself,in child its pointless
// calls exec builtin funciton with the first command in the command list
// cannot exit because it would exit the main process thus the entire program
int	exec_builtin_in_parent(t_mini *mini, int files[2])
{
	int	result;

	set_files(mini, mini->cmd_list, &files[0], &files[1]);
	dup2(files[0], STDIN_FILENO);
	dup2(files[1], STDOUT_FILENO);
	close_files(&files[0], &files[1]);
	result = exec_builtins(mini, mini->cmd_list);
	if (result == 0)
		return (0);
	else
		return (1);
}

// we have to understand we call it command but it can also be path
// it actually always is path to the executable file -> command
// 1. first check builtin functions - do function for this later
// 2. check for executables specified by path - eg. ./minishell, ../minishell
// 3. means that only place left to look for are the commands at $PATH
// free all resources at end before exit
// @returns nothing, exits with proper exit status of command
void	execute(t_mini *mini, t_cmd *cmd)
{
	int	result;

	result = 0;
	if (cmd->cmd)
	{
		if (is_builtin(cmd))
			result = exec_builtins(mini, cmd);
		else if (ft_strchr(cmd->cmd, '/'))
			result = exec_command_by_path(mini, cmd);
		else
			result = exec_shell_command(mini, cmd);
	}
	free_cmd_list(mini);
	free_arr(mini->env);
	free_int_arr(mini->pipes, mini->pids);
	exit(result);
}

// for each process set most recent infile and outfile from its redir struct
// set STDIN and STDOUT of each process to corresponding pipe or file
// if no file and no pipe, STDIN and STDOUT are kept as processes STDIN, STDOUT
// close pipes and files - ones we need are dupped so we do not need them now
// @returns nothing - execute() exits child process with proper exit status
void	exec_cmd(t_mini *mini, int **pipes, int files[], int i)
{
	t_cmd	*nthcmd;
	int		num_of_p;

	num_of_p = get_cmd_count(mini->cmd_list);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	set_termios(0);
	nthcmd = get_nth_command(mini->cmd_list, i);
	if (!nthcmd)
	{
		close_all_pipes(pipes, num_of_p);
		free_int_arr(mini->pipes, mini->pids);
		exit(mini_error(mini, create_msg("mini", "no cmd", NULL, NULL), 127));
	}
	if (cmd_evaluator(mini, nthcmd) == ERROR)
	{
		close_all_pipes(pipes, num_of_p);
		free_int_arr(mini->pipes, mini->pids);
		exit (1);
	}
	set_files(mini, nthcmd, &files[0], &files[1]);
	set_ins_outs(i, pipes, files, num_of_p);
	close_files(&files[0], &files[1]);
	close_all_pipes(pipes, num_of_p);
	execute(mini, nthcmd);
}

// check if builtin & 1 command - not open any processes, must be done in main
// in this case execute and return, not exit, as it would kill entire program
// create as many processes as commands in loop
// BUT each fork duplicates existing process so we end up in more
// always use only the child process by if pids[0] == 0 and execve/exit at end
// so it does not fork into its child processes
// each command in pipeline (=process) is executed independently
// failure of one does not prevent others from executing
// @returns executor returns exit status of its most recent process
int	executor(t_mini *mini, int num_of_p)
{
	int		files[2];
	int		i;

	if (init_int_arrs(mini, num_of_p) == ERROR)
		return (ERROR);
	i = -1;
	files[0] = STDIN_FILENO;
	files[1] = STDOUT_FILENO;
	if (num_of_p == 1 && is_builtin(mini->cmd_list))
		return (exec_builtin_in_parent(mini, files));
	if (open_pipes(mini->pipes, num_of_p) != 0)
		return (free_int_arr(mini->pipes, mini->pids), ERROR);
	signal(SIGINT, SIG_IGN);
	while (++i < num_of_p)
	{
		mini->pids[i] = fork();
		if (mini->pids[i] == -1)
			return (close_all_pipes(mini->pipes, num_of_p),
				perror("minishell"), 1);
		if (mini->pids[i] == 0)
			exec_cmd(mini, mini->pipes, files, i);
	}
	close_all_pipes(mini->pipes, num_of_p);
	return (set_exit_status(num_of_p, mini, mini->pids));
}
