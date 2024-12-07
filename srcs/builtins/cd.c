/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:18 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/05 23:18:33 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// @returns ERROR: if HOME was not set
// @returns 0: changing directory successful
// @returns ERROR: chdir returned -1 - error changing directory
int	cd_home_checks(t_mini *mini)
{
	int		result;
	char	*home;

	result = 0;
	home = getenv_local(mini->env, "HOME");
	if (!home)
	{
		set_executor_error_msg(mini, "cd", "HOME not set", NULL);
		return (ERROR);
	}
	if (chdir(home) == -1)
	{
		result = errno;
		perror("minishell");
	}
	free(home);
	return(result);
}

// @returns ERROR if any check fails, otherwise 0
int	cd_checks(char **args, t_mini *mini, char *path)
{
	if (get_arr_len(args) > 2)
	{
		set_executor_error_msg(mini, "cd", path, "too many agruments");
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
	char	*path;
	int		result;

	result = 0;
	path = cmd->args[1];
	if (path == NULL || !ft_strncmp(path, "~", 2))
		return (cd_home_checks(mini));
	if (!ft_strncmp(path, "-", 2))
	{
		if(chdir("..") == -1)
			result = mini_perror();
	}
	if (cd_checks(cmd->args, mini, path) == ERROR)
		return (ERROR);
	if (chdir(path) == -1)
		result = mini_perror();
	return(result);
}
