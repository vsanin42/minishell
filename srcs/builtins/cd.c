/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:18 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/26 11:42:15 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// changes directory, handles absolute, relative, no path
// FREE is done in caller function
// @returns 0 on successful change of directory, 1 on error
int	cd_builtin(t_mini *mini, t_cmd *cmd)
{
	char	*home;
	char	*path;

	printf("executing cd builtin\n");
	path = cmd->args[1];
	if (path == NULL || !ft_strncmp(path, "~", 1))
	{
		home = getenv_local(mini->env, "HOME");
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
		free(home);
		set_executor_error_msg(mini, "cd", "Error changing directory", NULL);
		return (ERROR);
	}
	if (get_arr_len(cmd->args) > 2)
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
	if (chdir(path) == 0) // directory and exists and succesfully changed
	{
		return (0);
	}
	return (ERROR);
}
