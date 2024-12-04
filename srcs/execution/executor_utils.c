/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:02:34 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/29 15:50:19 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* ERROR MESSAGE */

void	set_executor_error_msg(t_mini *mini, char *first, char *second, char *third)
{
	char *msg;
	char *old_msg;
	char *new_part;

	free(mini->error_msg);
	mini->error_msg = NULL;
	msg = ft_strdup("minishell: ");
	if (first)
	{
		old_msg = msg;
		new_part = ft_strdup(first);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	if (second)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(second);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	if (third)
	{
		old_msg = msg;
		new_part = ft_strdup(": ");
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
		old_msg = msg;
		new_part = ft_strdup(third);
		if (new_part)
		{
			msg = ft_strjoin(msg, new_part);
			free(new_part);
			new_part = NULL;
			free(old_msg);
			old_msg = NULL;
		}
	}
	mini->error_msg = msg;
}
