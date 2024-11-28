/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:41 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/28 19:01:43 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*echo_builder(char **args)
{
	char	**tmp;
	char	*res;
	char	*trimmed_res;

	trimmed_res = NULL;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	tmp = args;
	if (args[1] && !ft_strncmp(args[1], "-n", 2))
		tmp += 2;
	else
		tmp++;
	while (*tmp)
	{		
		res = str_append_space(res, *tmp);
		tmp++;
	}
	trimmed_res = ft_strtrim(res, " ");
	free(res);
	return (trimmed_res);
}

// prints all arguments after it, can have -n option
int	echo_builtin(t_mini *mini, t_cmd *cmd)
{
	char	*res;

	(void)mini;
	res = echo_builder(cmd->args);
	if (!res)
		return (ERROR);
	if (!cmd->redir)
	{
		write(1, res, ft_strlen(res));
		if (cmd->args[1] && ft_strncmp(cmd->args[1], "-n", 2))
			write(1, "\n", 1);
	}
	free(res);
	return (0);
}
