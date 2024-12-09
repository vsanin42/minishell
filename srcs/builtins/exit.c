/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:24:05 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/09 15:39:05 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// exits the program, optional argument status that indicates success/failure status of the shell session or script
// exit status 0 indicates success, other indicate error or abnormal termination
// if no argument is provided to exit, the default exit status is that of the last executed command.
// WE SHOULD PROBABLY ADD FREEING FOR GOOD PRACTICE EVEN THOUGH THE OS WILL RECLAIM MALLOCED MEMORY ANYWAYS
// exit_status must be between 0 and 255
void	exit_builtin(t_mini *mini)
{
	int	exit_status;

	write(1, "exit\n", 5);
	if (!mini)
		exit(0);
	free_arr(mini->env);
	free_token_list(mini);
	free_cmd_list(mini);
	exit_status = mini->exit_status;
	if (exit_status < 0 || exit_status > 255)
		exit(exit_status % 256);
	exit(exit_status);
}
