/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:35:40 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/21 22:39:23 by vsanin           ###   ########.fr       */
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

	if (!nc_init(&node, &args, &ahead))
		return (NULL);
	init_cmd_node(node);
	node->redir = find_redirs(token);
	while (token && token->type != TOKEN_PIPE)
	{
		if (first_entry(&token, &node, &args, &ahead) == ERROR)
			return (NULL);
		if (token->type == TOKEN_TEXT)
			*args++ = ft_strdup(token->value);
		if (!(*(args - 1)))
		{
			free(node->cmd);
			free_char_pp(ahead);
			free_redir(node->redir);
			return (free(node), NULL);
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
