/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:23:41 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/18 15:37:55 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks the validity of -n option
// valid: any number of 'n' after '-' (at least 1)
// invalid: anything other than 'n' after '-'
// @returns 0 if option is valid - don't print newline
int	echo_n_option(char **args, int i)
{
	char	*str;

	str = args[i];
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

int	echo_skip_n(char **tmp)
{
	int	i;

	i = 0;
	if (!tmp || !*tmp)
		return (ERROR);
	if (echo_n_option(tmp, i) == ERROR)
		return (ERROR);
	else
		return (0);
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
	if (!args[1])
		return (NULL);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	tmp = args + 1;
	while (!echo_skip_n(tmp))
		tmp++;
	while (*tmp)
	{
		res = str_append_space(res, *tmp);
		tmp++;
	}
	trimmed_res = ft_strdup(res);
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
	{
		write(1, "\n", 1);
		return (ERROR);
	}
	write(1, res, ft_strlen(res));
	if (cmd->args[1] && echo_n_option(cmd->args, 1))
		write(1, "\n", 1);
	free(res);
	return (0);
}
