/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:34:44 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/11 20:12:40 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// frees the program struct and possibly 2 strings so we save lines on norm
// writes error message to stderr
// @returns ERROR (1) as status code
// @param msg message describing the error
// @param mini struct containing all program-wide data needed to be freed
// @param str_to_free 3rd and 4th param that free a string and set it to NULL
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
