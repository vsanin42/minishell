/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:19:33 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/29 13:34:10 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(t_mini *mini)
{
	t_cmd	*cmd;

	cmd = mini->cmd_list;
	if (cmd && !cmd->next && cmd->cmd
		&& (!ft_strncmp(cmd->cmd, "cd", 2)
			|| !ft_strncmp(cmd->cmd, "pwd", 3)
			|| !ft_strncmp(cmd->cmd, "env", 3)
			|| !ft_strncmp(cmd->cmd, "export", 6)
			|| !ft_strncmp(cmd->cmd, "unset", 5)
			|| !ft_strncmp(cmd->cmd, "exit", 4)))
		return (1);
	return (0);
}
