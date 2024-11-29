/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:35:40 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/29 10:59:55 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// allocates size of array of arguments collected from text strings
// @returns the allocates array with empty spaces for arguments
char	**alloc_args(char **args, t_token *token)
{
	int len;

	len = get_ttokens_len(token);
	if (!args)
	{
		if (len > 0)
		{
			args = malloc(sizeof(char *) * (len + 1));
			if (!args)
				return (NULL);
			args[len] = NULL;
		}
	}
	return (args);
}

// this is called at the beginning of the new cmd loop at each token
// it checks if the token is text + if the node doesn't have the cmd name yet
// if both are correct, sets the cmd name and "if" will never be true again
// also allocates args and sets their head
// @returns: 0 on regular flow, ERROR on failed allocations
int	first_entry(t_token **token, t_cmd **node, char ***args, char ***ahead)
{
	if ((*token)->type == TOKEN_TEXT && !(*node)->cmd)
	{
		(*node)->cmd = ft_strdup((*token)->value);
		if (!((*node)->cmd))
			return (ERROR);
		*args = alloc_args(*args, *token);
		if (!(*args))
			return (free((*node)->cmd), ERROR);
		*ahead = *args;
	}
	return (0);
}

// collects tokens from token list into command or commands separated by pipe
// if no new node, free all previous nodes and return error
// if encounters pipe, starts creating new command
// @returns head of the command list
// @var previous stores previous node, so if it is pipe we can save it in redir
int	parser(t_mini *mini)
{
	t_cmd	*command_list;
	t_cmd	*new_node;
	t_token	*temp;

	new_node = NULL;
	command_list = NULL;
	temp = mini->token_list;
	while (temp)
	{
		new_node = new_cmd(temp);
		if (!new_node)
		{
			free_cmd_list(command_list);
			return (error_msg("Parser error", mini, 0, 0));
		}
		add_back_cmd(&command_list, new_node);
		while (temp && temp->type != TOKEN_PIPE)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
	mini->cmd_list = command_list;
	return (0);
}
