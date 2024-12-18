/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:31:36 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/18 13:03:52 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	s_error_msg(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

// prints msg and perror after
// ! msg must be allocated, will be freed
// returns errno as result
int	mini_perror(t_mini *mini, char *msg)
{
	int	result;

	(void)mini;
	result = errno;
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
		write(STDERR_FILENO, msg, ft_strlen(msg));
		write(STDERR_FILENO, "\n", 1);
		free(msg);
	}
	return (err);
}
