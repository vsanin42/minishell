/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:34:44 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/07 18:58:26 by zuzanapiaro      ###   ########.fr       */
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
		if (mini->token_list)
			free_token_list(mini);
		if (mini->cmd_list)
			free_cmd_list(mini);
		mini->token_list = NULL;
		mini->cmd_list = NULL;
		//free_arr(mini->env);
	}
	free(str_to_free_1);
	str_to_free_1 = NULL;
	free(str_to_free_2);
	str_to_free_2 = NULL;
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (ERROR);
}

void	s_error_msg(char *msg)
{
	printf("%s\n", msg);
}

void	validator_msg(t_mini *mini, char *object, char *msg)
{
	(void)mini;
	printf("minishell: %s: %s\n", object, msg);
}

int mini_perror(t_mini *mini)
{
	int result;

	result = errno;
	printf("errno: %d\n", errno);
	perror("minishell");
	return (result);
}
