/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 13:24:19 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/15 13:25:12 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_question(t_mini *mini, char *res, char *text, int *i)
{
	char	*oldres;
	char	*status;

	if (text[*i] == '?')
		(*i)++;
	oldres = res;
	status = ft_itoa(mini->exit_status);
	if (!status)
		return (res);
	res = ft_strjoin(res, status);
	if (!res)
		return (free(status), oldres);
	free(oldres);
	free(status);
	return (res);
}

char	*quickjoin(char *res)
{
	char	*oldres;

	oldres = res;
	res = ft_strjoin(res, "$");
	if (!res)
		return (NULL);
	free(oldres);
	return (res);
}
