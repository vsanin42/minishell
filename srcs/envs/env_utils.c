/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:40:19 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/10 14:51:35 by zpiarova         ###   ########.fr       */
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

	i = -1;
	env_path = getenv_local(mini->env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (free(env_path), NULL);
	while (paths[++i])
	{
		path_without_cmd = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_without_cmd, cmd);
		free(path_without_cmd);
		if (!path)
			return (free(env_path), NULL);
		if (access(path, F_OK) == 0)
			return (free(env_path), free_paths(paths, &i), path);
		free(path);
	}
	return (free(env_path), free_paths(paths, &i), free(paths), NULL);
}

// takes entire env string and finds index based on name in the env array
// @returns index if found or -1 if not found
int	get_env_index_by_name(char **envs, char *env_name)
{
	int		i;
	char	*curr_env_name;

	if (!envs || !(*envs) || !env_name)
		return (-1);
	i = -1;
	while (envs[++i])
	{
		curr_env_name = extract_env_name(envs[i]);
		if (!curr_env_name)
			return (-1);
		if (!ft_strncmp(curr_env_name, env_name, ft_strlen(env_name) + 1))
		{
			free(curr_env_name);
			curr_env_name = NULL;
			return (i);
		}
		free(curr_env_name);
		curr_env_name = NULL;
	}
	return (-1);
}

// replicates getenv function
// @returns allocated string, must be freed !!! or NULL or error
char	*getenv_local(char **envs, char *env_name)
{
	int		i;
	char	*curr_env_name;
	char	*value;

	if (!envs || !(*envs) || !env_name)
		return (NULL);
	value = NULL;
	i = -1;
	while (envs[++i])
	{
		curr_env_name = extract_env_name(envs[i]);
		if (!curr_env_name)
			return (NULL);
		if (!ft_strncmp(curr_env_name, env_name, ft_strlen(env_name) + 1))
		{
			value = ft_substr(envs[i], ft_strlen(env_name) + 1,
					(ft_strlen(envs[i]) - ft_strlen(env_name) - 1));
			free(curr_env_name);
			return (value);
		}
		free(curr_env_name);
	}
	return (NULL);
}

// takes an env string as NAME=value and extracts the NAME
// if no value, returns the NAME itself
// @returns name of env variable
char	*extract_env_name(char *env)
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

// checks if passed in env contains also value or only name(possibly ending in =)
// @returns 0 if has value, 1 if not
int has_env_value(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	if (env[i] == '\0' || (env[i] == '=' && env[i + 1] == '\0'))
		return (ERROR);
	return (0);
}

// checks whether env has valid name-only alnum or _, starting with letter or _
// @returns 0 if valid name, 1 if not
int	check_env_name(char *env)
{
	int	i;

	i = 0;
	if (env[i] >= '0' && env[i] <= '9')
		return (ERROR);
	while (env[i] && env[i] != '=')
	{
		if (!ft_isalnum(env[i]) && env[i] != '_')
			return (ERROR);
		i++;
	}
	return (0);
}
