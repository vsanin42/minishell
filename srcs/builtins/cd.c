/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:22:18 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/18 13:28:35 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_pwd_env(char **env)
{
	char	*cwd;
	char	*new_el;
	int		index;

	index = get_env_index_by_name(env, "PWD");
	cwd = get_current_directory();
	if (!cwd)
		return ;
	new_el = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!new_el)
		return ;
	change_arr_element(env, new_el, index);
	free(new_el);
}

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
		return (mini_error(mini, create_msg("minishell",
					"cd", "HOME not set", NULL), 1));
	if (chdir(home) == -1)
	{
		result = errno;
		mini_perror(mini, create_msg("minishell", "cd", path, NULL));
	}
	free(home);
	if (path && !ft_strncmp(path, "-\0", 2))
		return (pwd_builtin(mini, NULL));
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
	if (path == NULL || !ft_strncmp(path, "~\0", 2)
		|| !ft_strncmp(path, "-\0", 2))
		return (cd_home(mini, path));
	if (get_arr_len(cmd->args) > 2)
		return (mini_error(mini, create_msg("minishell", cmd->cmd,
					"too many argments", NULL), 1));
	if (!ft_strncmp(path, "-\0", 2))
	{
		if (chdir("..") == -1)
		{
			result = mini_perror(mini, NULL);
			return (result);
		}
	}
	if (chdir(path) == -1)
	{
		result = mini_perror(mini, create_msg("minishell", "cd", path, NULL));
		return (result);
	}
	return (update_pwd_env(mini->env), result);
}
