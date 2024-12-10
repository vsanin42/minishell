/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:19:33 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/10 14:23:26 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (cmd && cmd->cmd
		&& (!ft_strncmp(cmd->cmd, "cd", 3)
			|| !ft_strncmp(cmd->cmd, "pwd", 4)
			|| !ft_strncmp(cmd->cmd, "env", 4)
			|| !ft_strncmp(cmd->cmd, "export", 7)
			|| !ft_strncmp(cmd->cmd, "unset", 6)
			|| !ft_strncmp(cmd->cmd, "echo", 5)
			|| !ft_strncmp(cmd->cmd, "exit", 5)))
		return (1);
	return (0);
}
