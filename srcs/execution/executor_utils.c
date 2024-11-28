/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:02:34 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/25 19:59:40 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_cmd_count(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		i += 1;
		cmd = cmd->next;
	}
	return (i);

}

t_cmd	*get_nth_command(t_cmd *cmdhead, int n)
{
	int	i;

	i = 0;
	if (!cmdhead)
	{
		return (NULL);
	}
	while (i < n && cmdhead)
	{
		cmdhead = cmdhead->next;
		i++;
	}
	if (i == n)
		return (cmdhead);
	else
		return (NULL);
}
