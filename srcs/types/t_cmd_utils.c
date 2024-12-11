/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:55:56 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/10 17:08:16 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// frees the command list
void	free_cmd_list(t_mini *mini)
{
	t_cmd	*temp;
	t_cmd	*node;

	node = mini->cmd_list;
	while (node)
	{
		temp = node;
		if (node->cmd)
			free(node->cmd);
		if (node->args)
			free_arr(node->args);
		if (node->redir)
			free_redir(node->redir);
		node = node->next;
		free(temp);
	}
	node = NULL;
	temp = NULL;
	mini->cmd_list = NULL;
}

void	free_cmd_nodes(t_cmd *node)
{
	t_cmd	*temp;

	while (node)
	{
		temp = node;
		if (node->cmd)
			free(node->cmd);
		if (node->args)
			free_arr(node->args);
		if (node->redir)
			free_redir(node->redir);
		node = node->next;
		free(temp);
	}
	node = NULL;
	temp = NULL;
}

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
