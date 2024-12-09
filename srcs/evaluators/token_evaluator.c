/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_evaluator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/09 15:51:23 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_unexpected_tokens(t_mini *mini)
{
	t_token	*temp;

	if (!mini->token_list)
		return (ERROR);
	temp = mini->token_list;
	if (temp->type == TOKEN_PIPE)
		return (validator_msg(mini, "syntax error near unexpected token", temp->value), 2);
	while (temp)
	{
		if (temp->type == TOKEN_PIPE && temp->next && temp->next->type != TOKEN_PIPE)
		{
			temp = temp->next;
			continue ;
		}
		else if (temp->type == TOKEN_PIPE && (!temp->next || temp->next->type == TOKEN_PIPE))
			return (validator_msg(mini, "syntax error near unexpected token", temp->value), 2);
		else if (temp->type != TOKEN_TEXT && !temp->next)
			return (validator_msg(mini, "syntax error near unexpected token", "`newline'"), 2);
		else if (temp->type != TOKEN_TEXT && temp->next->type == TOKEN_PIPE)
			return (validator_msg(mini, "syntax error near unexpected token", temp->next->value), 2);
		else if (temp->type != TOKEN_TEXT && temp->next->type != TOKEN_TEXT)
			return (validator_msg(mini, "syntax error near unexpected token", temp->next->value), 2);
		temp = temp->next;
	}
	return (0);
}

int	token_evaluator(t_mini *mini)
{
	return (check_unexpected_tokens(mini));
}
