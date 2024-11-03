/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 16:06:31 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/03 16:52:27 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// sets the infile for input instead of STDIN
// must close the file afterwards
// @returns 0 on success, -1 on error
int	redirect_input(char *file)
{
	int	infile;

	infile = open(file, O_RDONLY/* , 0777 */);
	if (infile == -1)
		return(-1);
	if (dup2(infile, STDIN_FILENO) == -1)
	{
		close(infile);
		return (-1);
	}
	close(infile);
	return (0);
}
