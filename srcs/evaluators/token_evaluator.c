/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_evaluator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/28 17:53:30 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_unexpected_tokens(t_mini *mini, t_token *token)
{
	t_token	*temp;

	if (!token)
		return (NULL);
	temp = token;
	while (temp)
	{
		if (temp->type == TOKEN_PIPE && temp->next->type != TOKEN_TEXT)
			validator_msg(mini, "syntax error near unexpected token", temp->value);
		else if (temp->type != TOKEN_TEXT && temp->next->type == TOKEN_PIPE)
			validator_msg(mini, "syntax error near unexpected token", temp->next->value);
		else if (temp->type != TOKEN_TEXT && temp->next->type != TYPE_TEXT)
			validator_msg(mini, "syntax error near unexpected token", temp->next->value);
		temp = temp->next;
	}
}
