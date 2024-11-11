/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:34:44 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/11 17:25:42 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// might need a better way to exit later because this doesn't free anything
int	error_msg(char *msg, t_mini *mini, char *str_to_free_1, char *str_to_free_2)
{
	if (mini)
	{
		free_token_list(mini->token_list);
		free_cmd_list(mini->cmd_list);
	}
	free(str_to_free_1);
	str_to_free_1 = NULL;
	free(str_to_free_2);
	str_to_free_2 = NULL;
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}
