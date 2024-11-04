/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 16:06:31 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/04 20:20:27 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// sets the infile for input instead of STDIN
// must close the file afterwards
// @returns 0 on success, -1 on error
int	redirect_input(char *file)
{
	int	infile;

	infile = open(file, O_RDONLY, 0777);
	if (infile == -1)
		return(-1);
	if (dup2(infile, 0) == -1)
	{
		close(infile);
		return (-1);
	}
	close(infile);
	return (0);
}
