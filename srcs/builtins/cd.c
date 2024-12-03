/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:18 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/03 16:34:37 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// @returns ERROR: if HOME was not set
// @returns 0: changing directory successful
// @returns ERROR: chdir returned -1 - error changing directory
int	cd_home_checks(char *home, t_mini *mini)
{
	if (!home)
	{
		set_executor_error_msg(mini, "cd", "HOME not set", NULL);
		return (ERROR);
	}
	if (chdir(home) == 0)
	{
		free(home);
		return (0);
	}
	else
	{
		free(home);
		set_executor_error_msg(mini, "cd", "Error changing directory", NULL);
		return (ERROR);
	}
}

// @returns ERROR if any check fails, otherwise 0
int	cd_checks(char **args, t_mini *mini, char *path)
{
	if (get_arr_len(args) > 2)
	{
		set_executor_error_msg(mini, "cd", path, "too many agruments");
		return (ERROR);
	}
	if (!is_directory(path))
	{
		set_executor_error_msg(mini, "cd", path, "Not a directory");
		return (ERROR);
	}
	if (is_directory(path) < 0)
	{
		set_executor_error_msg(mini, "cd", path, "No such file or directory");
		return (ERROR);
	}
	return (0);
}

// changes directory, handles absolute, relative, no path
// FREE is done in caller function
// !!!: strncmp compares ~ AND \0 to make sure it's only ~
// @returns 0 on successful change of directory, 1 on error
int	cd_builtin(t_mini *mini, t_cmd *cmd)
{
	char	*home;
	char	*path;

	path = cmd->args[1];
	if (path == NULL || !ft_strncmp(path, "~", 2))
	{
		home = getenv_local(mini->env, "HOME");
		return (cd_home_checks(home, mini));
	}
	if (!ft_strncmp(path, "-", 2))
	{
		chdir("..");
		return (0);
	}
	if (cd_checks(cmd->args, mini, path) == ERROR)
		return (ERROR);
	if (chdir(path) == 0)
		return (0);
	return (ERROR);
}
