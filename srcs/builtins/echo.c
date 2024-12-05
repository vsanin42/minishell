/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:41 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/05 01:38:57 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks the validity of -n option
// valid: any number of 'n' after '-' (at least 1)
// invalid: anything other than 'n' after '-'
// @returns 0 if option is valid - don't print newline
int	echo_n_option(char **args)
{
	char	*str;

	str = args[1];
	if (*str == '-')
	{
		str++;
		if (*str == '\0')
			return (ERROR);
		while (*str)
		{
			if (*str != 'n')
				return (ERROR);
			str++;
		}
		return (0);
	}
	return (ERROR);
}

// fills a result string with all arguments
// skips the -n/-nnn* option if validated
// @returns collected arguments in a string joined by spaces
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
	if (args[1] && !echo_n_option(args))
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

// prints all arguments after the command
// if -n/-nnn* option is specified, the trailing newline is not printed
// @returns 0 on success, 1 on error
int	echo_builtin(t_mini *mini, t_cmd *cmd)
{
	char	*res;

	(void)mini;
	res = echo_builder(cmd->args);
	if (!res)
		return (ERROR);
	write(1, res, ft_strlen(res));
	if (cmd->args[1] && echo_n_option(cmd->args))
		write(1, "\n", 1);
	free(res);
	return (0);
}
