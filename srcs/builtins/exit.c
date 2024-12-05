/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:24:05 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/05 12:47:40 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// exits program
// @param status(opt.) indicates success/failure status of shell session/script
// exit status 0 indicates success, else error or abnormal termination
// if no argument is provided, it exits with status of last executed command
// WE SHOULD PROBABLY ADD FREEING FOR GOOD PRACTICE
// EVEN THOUGH THE OS WILL RECLAIM MALLOCED MEMORY ANYWAYS
// exit_status is always between 0 and 255
void	exit_builtin(t_mini *mini)
{
	int	exit_status;

	write(1, "exit\n", 5);
	if (!mini)
		exit(0);
	free_arr(mini->env);
	free_token_list(mini);
	free_cmd_list(mini);
	free(mini->error_msg);
	exit_status = mini->exit_status;
	if (exit_status < 0 || exit_status > 255)
		exit(exit_status % 256);
	exit(exit_status);
}
