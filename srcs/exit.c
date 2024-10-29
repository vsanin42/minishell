/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:34:44 by vsanin            #+#    #+#             */
/*   Updated: 2024/10/29 14:38:30 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// might need a better way to exit later because this doesn't free anything
int	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}