/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:46:42 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/10 17:05:19 by zpiarova         ###   ########.fr       */
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
			dup_value = ft_strdup(token->value);
			if (!dup_value)
				return (free_cmd_nodes(node), NULL);
			*args = dup_value;
			args++;
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
