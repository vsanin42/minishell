/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:40:47 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/14 20:24:20 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// handle pipe RD at first node of command list or pipe WR at last node
// if there is RD pipe at first node, it is error
// if there is WR pipe at the last node, it is error
// @returns 0 if all was good, 1 if there were pipes where they shouldnt be
int	check_pipes(t_mini *mini)
{
	t_cmd *cmd;

	cmd = mini->cmd_list;
	if (cmd)
	{
		if (cmd->redir && !ft_strncmp(cmd->redir->file, "WR", 2))
			return (validator_msg(mini, "syntax error near unexpected token", "'|'"), ERROR);
		while (cmd->next)
			cmd = cmd->next;
		if (cmd->redir && !ft_strncmp(cmd->redir->file, "RD", 2))
			return (validator_msg(mini, "parse error:", "cannot end in '|'"), ERROR);
	}
	return (0);
}

// evaluate files - if exist and have correct permissions
// @returns 0 if all good, 1 if there was some error
int	validate_files(t_mini *mini)
{
	t_cmd	*cmd;
	t_redir	*red;

	cmd = mini->cmd_list;
	while (cmd)
	{
		red = cmd->redir;
		while (red)
		{
			// infile file does not exist
			if (red->type == TOKEN_REDIRIN && is_readable_file(red->file) == -1 && errno == ENOENT)
				return (validator_msg(mini, red->file, ": No such file or directory"), ERROR);
			// infile exists but error checking its permissions
			else if (red->type == TOKEN_REDIRIN && is_readable_file(red->file) == -1 && errno != ENOENT)
				return (validator_msg(mini, red->file, ": Error checking file permissions"), ERROR);
			// infile exists but doesnt have permissions
			else if (red->type == TOKEN_REDIRIN && (is_readable_file(red->file) == 0))
				return (validator_msg(mini, red->file, ": Permission denied"), ERROR);
			// outfile exists but error checking its permissions
			else if ((red->type == TOKEN_REDIROUT || red->type == TOKEN_APPEND) && is_writable_file(red->file) == -1 && errno != ENOENT)
				return (validator_msg(mini, red->file, ": Error checking file permissions"), ERROR);
			// outfile has write permission - it CAN not exist, it will be created
			else if ((red->type == TOKEN_REDIROUT || red->type == TOKEN_APPEND)  && (is_writable_file(red->file) == 0))
				return (validator_msg(mini, red->file, ": Permission denied"), ERROR);
			red = red->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

// evaluate COMMANDS/EXECUTABLES - if exist and have correct permissions

// remove NULLs from not-expanded envs in args arrays

// handle not-expaned command names - use first valid argument as command

// checks all received parameters and only if OK program continues to execution
// @returns 0 if all is OK, 1 on error
int	evaluator(t_mini *mini)
{
	if (check_pipes(mini) == 0
		&& validate_files(mini) == 0
		)
		return (0);
	return (1);
}
