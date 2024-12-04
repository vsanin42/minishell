/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_evaluator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:40:47 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/29 12:48:54 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// evaluate files for existance and correct permissions
// possible cases:
// 1. infile file does not exist
// 2. infile exists but error checking its permissions
// 3. infile exists but doesnt have permissions
// 4. outfile exists but error checking its permissions
// 5. outfile has write permission - it CAN not exist, it will be created
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
			if (red->type == TOKEN_REDIRIN && is_readable_file(red->file) == -1 && errno == ENOENT)
				return (validator_msg(mini, red->file, "No such file or directory"), ERROR);
			else if (red->type == TOKEN_REDIRIN && is_readable_file(red->file) == -1 && errno != ENOENT)
				return (validator_msg(mini, red->file, "Error checking file permissions"), ERROR);
			else if (red->type == TOKEN_REDIRIN && (is_readable_file(red->file) == 0))
				return (validator_msg(mini, red->file, "Permission denied"), ERROR);
			else if ((red->type == TOKEN_REDIROUT || red->type == TOKEN_APPEND) && is_writable_file(red->file) == -1 && errno != ENOENT)
				return (validator_msg(mini, red->file, "Error checking file permissions"), ERROR);
			else if ((red->type == TOKEN_REDIROUT || red->type == TOKEN_APPEND)  && (is_writable_file(red->file) == 0))
				return (validator_msg(mini, red->file, "Permission denied"), ERROR);
			red = red->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

// checks all received parameters and only if OK program continues to execution
// @returns 0 if all is OK, 1 on error
int	cmd_evaluator(t_mini *mini)
{
	if (validate_files(mini) == 0)
		return (0);
	return (1);
}
