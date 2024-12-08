/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:19:33 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/07 23:56:47 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (cmd && cmd->cmd
		&& (!ft_strncmp(cmd->cmd, "cd", 2)
			|| !ft_strncmp(cmd->cmd, "pwd", 3)
			|| !ft_strncmp(cmd->cmd, "env", 3)
			|| !ft_strncmp(cmd->cmd, "export", 6)
			|| !ft_strncmp(cmd->cmd, "unset", 5)
			|| !ft_strncmp(cmd->cmd, "echo", 4)
			|| !ft_strncmp(cmd->cmd, "exit", 4)))
		return (1);
	return (0);
}
