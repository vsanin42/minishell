/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:40:19 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/04 11:24:26 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// receives string value WITHOUT $ that should be checked if it exists as env
// @returns expanded allocated value if found, NULL if not found,
// or allocated "$" if no value to check
char	*process_local_env(t_mini *mini, char *name)
{
	char	*res;

	if (!name)
		res = ft_strdup("$");
	else
	{
		res = getenv_local(mini->env, name);
		if (!res)
			return (NULL);
	}
	return (res);
}

void	free_paths(char **paths, int *i)
{
	*i = -1;
	while (paths[++(*i)])
		free(paths[*i]);
}

// env PATH: for commands/programs - searches for executable in each path from PATH env
// 1. find environment variable PATH
// 2. path_env consists of all possible paths where commands are located
//    on the machine, separated by colon :
// 3. separate this entire string of paths into array called paths
// 4. for each of these paths:
//   1. append "\" to the end of the path
//   2. append the command (file) at the end of the path
//   3. cheeck if file at this path exists with access()
//   4. free all allocated memory properly
char	*get_path_env(t_mini *mini, char *cmd)
{
	char	**paths;
	char	*path;
	int		i;
	char	*path_without_cmd;
	char	*env_path;

	i = 0;
	env_path = getenv_local(mini->env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	free(env_path);
	while (paths[i])
	{
		path_without_cmd = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_without_cmd, cmd);
		free(path_without_cmd);
		path_without_cmd = NULL;
		if (!path)
			return (NULL);
		if (access(path, F_OK) == 0)
		{
			free_paths(paths, &i);
			return (path);
		}
		free(path);
		i++;
	}
	free_paths(paths, &i);
	free(paths);
	return (NULL);
}

int	get_env_index(char **envs, char *env_name)
{
	int		i;
	char	*curr_env_name;

	if (!envs || !(*envs) || !env_name)
		return (-1);
	i = 0;
	while (envs[i])
	{
		curr_env_name = get_env_name(envs[i]);
		if (!curr_env_name)
			return (-1);
		if (!ft_strncmp(curr_env_name, env_name, ft_strlen(curr_env_name)))
		{
			free(curr_env_name);
			curr_env_name = NULL;
			return (i);
		}
		free(curr_env_name);
		curr_env_name = NULL;
		i++;
	}
	return (-1);
}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// strncmp based on the length of ENV_NAME,
// not the ones we iterate through inside env list
// otherwise USER matches USERaaa when it's invalid
char	*getenv_local(char **envs, char *env_name)
{
	int		i;
	char	*curr_env_name;
	char	*value;
	int		name_len;

	if (!envs || !(*envs) || !env_name)
		return (NULL);
	value = NULL;
	i = -1;
	name_len = ft_strlen(env_name);
	while (envs[++i])
	{
		curr_env_name = get_env_name(envs[i]);
		if (!curr_env_name)
			return (NULL);
		// printf("env found: %s\n", curr_env_name);
		// printf("env to match: %s\n\n", env_name);
		if (!ft_strncmp(curr_env_name, env_name, name_len))
		{
			value = ft_substr(envs[i], name_len + 1,
					(ft_strlen(envs[i]) - name_len - 1));
			free(curr_env_name);
			return (value);
		}
		free(curr_env_name);
	}
	return (NULL);
}

char	*get_env_name(char *env)
{
	int		i;
	char	*res;

	i = 0;
	if (!env)
		return (NULL);
	res = NULL;
	while (env[i])
	{
		if (env[i] == '=')
		{
			res = ft_substr(env, 0, i);
			return (res);
		}
		i++;
	}
	res = ft_strdup(env);
	return (res);
}
