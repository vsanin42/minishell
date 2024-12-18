/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:24:05 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/18 13:30:27 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// exits the program
// exit status 0 indicates success
// other indicate error or abnormal termination
// default is exit status of last executed command
// exit_status must be between 0 and 255
// if program exits with 128, means exit was called with code over 128
int	exit_builtin(t_mini *mini, t_cmd *cmd)
{
	int	exit_status;

	write(1, "exit\n", 5);
	if (!mini)
		exit(0);
	if (get_arr_len(cmd->args) == 2)
	{
		if (ft_atoi(cmd->args[1]) == 0 && ft_strncmp(cmd->args[1], "0\0", 2))
			exit_status = mini_error(mini, create_msg("minishell", cmd->cmd,
						cmd->args[1], "numeric argument required"), 2);
		else
			exit_status = ft_atoi(cmd->args[1]);
	}
	else if (get_arr_len(cmd->args) >= 2)
		return (mini_error(mini, create_msg("minishell", cmd->cmd,
					"too many arguments", NULL), 1));
	else
		exit_status = 0;
	free_arr(mini->env);
	free_token_list(mini);
	free_cmd_list(mini);
	if (exit_status < 0 || exit_status > 255)
		exit(exit_status % 256);
	exit(exit_status);
	return (exit_status);
}
