/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:35:40 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/14 20:33:09 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// initializes cmd_nodes values cmd, args, redir, next to NULL
void	init_cmd_node(t_cmd *node)
{
	node->cmd = NULL;
	node->args = NULL;
	node->redir = NULL;
	node->next = NULL;
}

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

// adjusted ft_lstnew = allocates new struct cmd and assigns its values
// collects tokens into one command until encounters end or a pipe
// store first text token as command, the rest into arguments array
// store every text node after redir type into redir struct
// @returns created cmd node in command list
// @param token token from which we start collecting tokens into command
// @param previous exists if we had a pipe before our command
t_cmd	*new_cmd(t_token *token, t_token *previous)
{
	t_cmd	*node;
	char	**args;
	char	**args_head;

	args = NULL;
	args_head = NULL;
	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	init_cmd_node(node);
	node->redir = find_redirs(token, previous);
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_TEXT && !node->cmd)
		{
			node->cmd = ft_strdup(token->value);
			args = alloc_args(args, token);
			args_head = args;
		}
		if (token->type == TOKEN_TEXT) //&& node->cmd)
			*args++ = ft_strdup(token->value);
		token = token->next;
	}
	node->args = args_head;
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

// collects tokens from token list into command or commands separated by pipe
// if encounters pipe, starts creating new command
// @returns head of the command list
// @var previous stores previous node, so if it is pipe we can save it in redir
t_cmd	*parser(t_mini *mini)
{
	t_cmd	*command_list;
	t_cmd	*new_node;
	t_token	*temp;
	t_token	*previous;

	new_node = NULL;
	previous = NULL;
	command_list = NULL;
	temp = mini->token_list;
	while (temp) // temp is first token at start, or pipe, or we found end so null so it will not run anymore
	{
		new_node = new_cmd(temp, previous);
		if (!new_node)
			return (NULL);
		add_back_cmd(&command_list, new_node);
		while (temp && temp->type != TOKEN_PIPE)
			temp = temp->next;
		if (temp)
		{
			previous = temp;
		 	temp = temp->next;
		}
	}
	return (command_list);
}
