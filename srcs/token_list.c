/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:57:39 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/21 17:59:24 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// adjusted ft_lstnew = allocates memory for t_token node and fills in values
// @returns created node of the token list
t_token	*new_token(char *value, t_type type)
{
	t_token	*node;

	node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = value;
	node->type = type;
	node->next = NULL;
	return (node);
}

// adjusted ft_lstadd_back = appends created node to end of token list
// @returns nothing
void	add_back_token(t_token **lst, t_token *new)
{
	t_token	*temp;

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

t_token	*remove_null_tokens(t_token *token)
{
	t_token	*head;
	t_token	*temp;

	head = token;
	while (head && (!head->value || head->value[0] == '\0'))
	{
		temp = head;
		head = head->next;
		free(temp->value); // just in case
 		free(temp);
	}
	token = head;
	while (token && token->next)
	{
		if (!token->next->value || token->next->value[0] == '\0')
		{	
			temp = token->next;
			token->next = token->next->next;
			free(temp->value); // just in case
 			free(temp);
		}
		else
			token = token->next;
	}
	return (head);
}
