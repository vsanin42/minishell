/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:24:05 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/25 19:24:13 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// exits the program, optional argument status that indicates success/failure status of the shell session or script
// exit status 0 indicates success, other indicate error or abnormal termination
// if no argument is provided to exit, the default exit status is that of the last executed command.
// WE SHOULD PROBABLY ADD FREEING FOR GOOD PRACTICE EVEN THOUGH THE OS WILL RECLAIM MALLOCED MEMORY ANYWAYS
void	exit_builtin(char *status)
{
	int	exit_status;

	if (status == NULL) // if status is not specified, exit with the most recent status code of the program  - CHANGE from 0
		exit(0);
	exit_status = ft_atoi(status);
	if (exit_status < 0 || exit_status > 255) // exit_status must be between 0 and 255, else exit as error
		exit(exit_status % 256);
	exit(exit_status);
}
