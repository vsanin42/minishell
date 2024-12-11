/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:07:03 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/10 17:08:20 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*remove_null_tokens(t_token *token)
{
	t_token	*head;
	t_token	*temp;

	head = token;
	while (head && (!head->value || head->value[0] == '\0'))
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
	token = head;
	while (token && token->next)
	{
		if (!token->next->value || token->next->value[0] == '\0')
		{
			temp = token->next;
			token->next = token->next->next;
			free(temp->value);
			free(temp);
		}
		else
			token = token->next;
	}
	return (head);
}

// counts text tokens in token list after input token and before end or pipe
// @returns number of text tokens after token from parameter
int	get_ttokens_len(t_token	*token)
{
	int		i;
	t_token	*temp;

	i = 0;
	temp = token;
	while (temp && temp->type != TOKEN_PIPE)
	{
		if (temp->type == TOKEN_TEXT)
			i++;
		temp = temp->next;
	}
	return (i);
}

// frees the token list
void	free_token_list(t_mini *mini)
{
	t_token	*temp;
	t_token	*token;

	token = mini->token_list;
	while (token)
	{
		temp = token;
		free(token->value);
		token = token->next;
		free(temp);
		temp = NULL;
	}
	token = NULL;
	mini->token_list = NULL;
}
