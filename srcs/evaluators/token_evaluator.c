/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_evaluator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:38:45 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/28 23:14:09 by zuzanapiaro      ###   ########.fr       */
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
		return (validator_msg(mini, "syntax error near unexpected token", temp->value), ERROR);
	while (temp)
	{
		if (temp->type == TOKEN_PIPE && temp->next->type != TOKEN_PIPE)
		{
			temp = temp->next;
			continue ;
		}
		else if (temp->type == TOKEN_PIPE && !temp->next)
			return (validator_msg(mini, "syntax error near unexpected token", temp->value), ERROR);
		else if (temp->type != TOKEN_TEXT && !temp->next)
			return (validator_msg(mini, "syntax error near unexpected token", "`newline'"), ERROR);
		else if (temp->type != TOKEN_TEXT && temp->next->type == TOKEN_PIPE)
			return (validator_msg(mini, "syntax error near unexpected token", temp->next->value), ERROR);
		else if (temp->type != TOKEN_TEXT && temp->next->type != TOKEN_TEXT)
			return (validator_msg(mini, "syntax error near unexpected token", temp->next->value), ERROR);
		temp = temp->next;
	}
	return (0);
}

int	token_evaluator(t_mini *mini)
{
	return (check_unexpected_tokens(mini));
}
