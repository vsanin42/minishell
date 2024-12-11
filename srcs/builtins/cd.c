/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:18 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/10 18:13:03 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// @returns ERROR: if HOME was not set
// @returns 0: changing directory successful
// @returns ERROR: chdir returned -1 - error changing directory
int	cd_home(t_mini *mini, char *path)
{
	int		result;
	char	*home;

	result = 0;
	home = getenv_local(mini->env, "HOME");
	if (!home)
		return (mini_error(mini, create_msg("minishell", "cd", "HOME not set", NULL), 1));
	if (chdir(home) == -1)
	{
		result = errno;
		mini_perror(mini, create_msg("minishell", "cd", path, NULL));
	}
	free(home);
	return (result);
}

// changes directory, handles absolute, relative, no path
// FREE is done in caller function
// @returns 0 on successful change of directory, 1 on error
int	cd_builtin(t_mini *mini, t_cmd *cmd)
{
	char	*path;
	int		result;

	result = 0;
	path = cmd->args[1];
	if (path == NULL || !ft_strncmp(path, "~\0", 2))
		return (cd_home(mini, path));
	if (!ft_strncmp(path, "-", 2))
	{
		if (chdir("..") == -1)
		{
			result = mini_perror(mini, NULL);
			return (result);
		}
	}
	if (chdir(path) == -1)
		result = mini_perror(mini, create_msg("minishell--", "cd", path, NULL));
	return (result);
}
