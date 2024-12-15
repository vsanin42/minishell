/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:31:36 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/15 13:32:29 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	s_error_msg(char *msg)
{
	printf("%s\n", msg);
}

void	validator_msg(t_mini *mini, char *object, char *msg)
{
	(void)mini;
	printf("minishell: %s: %s\n", object, msg);
}

int	mini_perror(t_mini *mini, char *msg)
{
	int	result;

	(void)mini;
	result = errno;
	//mini->exit_status = errno;
	if (msg)
		perror(msg);
	else
		perror("minishell");
	free(msg);
	return (result);
}

// called as perror but where errno is not set by system call or other function
// call create_msg as msg and it will be written to STDERR and freed
// sets error code passed as 3rd argument as error status to mini
int	mini_error(t_mini *mini, char *msg, int err)
{
	(void)mini;
	if (msg)
	{
		write(2, msg, ft_strlen(msg));
		write(2, "\n", 1);
		free(msg);
	}
	return (err);
}
