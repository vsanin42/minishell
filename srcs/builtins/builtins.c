/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:19:33 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/17 20:25:06 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (cmd && cmd->cmd
		&& (!ft_strncmp(cmd->cmd, "cd\0", 3)
			|| !ft_strncmp(cmd->cmd, "pwd\0", 4)
			|| !ft_strncmp(cmd->cmd, "env\0", 4)
			|| !ft_strncmp(cmd->cmd, "export\0", 7)
			|| !ft_strncmp(cmd->cmd, "unset\0", 6)
			|| !ft_strncmp(cmd->cmd, "echo\0", 5)
			|| !ft_strncmp(cmd->cmd, "exit\0", 5)))
		return (1);
	return (0);
}
