/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:10:30 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/05 15:12:51 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void perror_msg(char *el)
{
	char	*temp;

	temp = "minishell: ";
	temp = ft_strjoin(temp, el);
	if (!temp)
		temp = "minishell: ";
	perror(temp);
	free(temp);
	temp = NULL;
}
