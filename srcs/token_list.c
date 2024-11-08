/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:57:39 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/08 13:58:01 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

t_token	*new_token(char *value, t_token_type type)
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

void	clear_token_list(t_token *token)
{
	t_token *temp;

	while (token)
	{
		temp = token;
		free((token)->value);
		token = (token)->next;
		free(temp);
	}
	token = NULL;
}
