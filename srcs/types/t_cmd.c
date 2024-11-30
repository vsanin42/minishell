/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:46:42 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/29 15:49:47 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// initializes cmd_nodes values cmd, args, redir, next to NULL
void	init_cmd_node(t_cmd *node)
{
	node->cmd = NULL;
	node->args = NULL;
	node->redir = NULL;
	node->next = NULL;
}

// quick init function to init node, args and head
// @returns: 0 if malloc fails, 1 regular
int	nc_init(t_cmd **node, char ***args, char ***ahead)
{
	*args = NULL;
	*ahead = NULL;
	*node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!*node)
		return (0);
	return (1);
}

// adjusted ft_lstnew = allocates new struct cmd and assigns its values
// collects tokens into one command until encounters end or a pipe
// store first text token as command, then it + the rest into arguments array
// store every text node after redir type into redir struct
// if previous strduped *args check fails,
// it frees cmd name, redir, collected args and node itself
// the rest of cmd structs are cleaned outside in that case
// @returns created cmd node in command list
// @param token token from which we start collecting tokens into command
// @param previous exists if we had a pipe before our command
t_cmd	*new_cmd(t_token *token)
{
	t_cmd	*node;
	char	**args;
	char	**ahead;
	char	*dup_value;

	if (!nc_init(&node, &args, &ahead))
		return (NULL);
	init_cmd_node(node);
	node->redir = find_redirs(token);
	while (token && token->type != TOKEN_PIPE)
	{
		if (first_entry(&token, &node, &args, &ahead) == ERROR)
			return (NULL);
		if (token->type == TOKEN_TEXT)
		{
			dup_value = ft_strdup(token->value);  // Duplicate the value
			if (!dup_value)  // Check if duplication failed
			{
				free(node->cmd);
				free_arr(ahead);
				free_redir(node->redir);
				free(node);
				return (NULL);
			}
			*args = dup_value;  // Assign the duplicated value
			args++;  // Increment args only after the value is assigned
		}
		token = token->next;
	}
	node->args = ahead;
	return (node);
}

// adjusted ft_lstadd_back = appends created node to a cmd list
void	add_back_cmd(t_cmd **lst, t_cmd *new)
{
	t_cmd	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

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
