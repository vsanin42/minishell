/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/25 16:30:40 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// all builtins return 0 on success and 1 on error
// they do not quit the program in error, mut be handled by caller function
// TODO: must update exit status at end of each of these functions

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
		home = getenv("HOME");
		if (!home)
		{
			set_executor_error_msg(mini, "cd", "HOME not set", NULL);
			return (ERROR);
		}
		if (chdir(home) == 0)
			return (0);
		set_executor_error_msg(mini, "cd", "Error changing directory", NULL);
		return (ERROR);
	}
	if (get_args_len(cmd) > 2)
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

// duplicates pwd - WORKS
// call for type EXECUTABLE
// @returns 0 on success, 1 on error
int	pwd_builtin(t_mini *mini, t_cmd *cmd)
{
	char	*cwd;

	(void)cmd;
	cwd = get_current_directory(); // testing finding a path when we will be expecting path type
	if (!cwd)
	{
		set_executor_error_msg(mini, "pwd", "cwd not found", NULL);
		return (ERROR);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}

// exits the program, optional argument status that indicates success/failure status of the shell session or script
// exit status 0 indicates success, other indicate error or abnormal termination
// if no argument is provided to exit, the default exit status is that of the last executed command.
// WE SHOULD PROBABLY ADD FREEING FOR GOOD PRACTICE EVEN THOUGH THE OS WILL RECLAIM MALLOCED MEMORY ANYWAYS
void	exit_builtin(char *status)
{
	int	exit_status;

	if (status == NULL) // if status is not specified, exit with the most recent status code of the program  - CHANGE 0
		exit(0);
	exit_status = atoi(status);
	if (exit_status < 0 || exit_status > 255) // exit_status must be between 0 and 255, else exit as error
		exit(exit_status % 256);
	exit(exit_status);
}

// returns value of $ENV, check if it really is ENV happens before it is called
// when calling if we are calling echo $non-existent-env, it prints newline if it is alone, and nothing if there is anything else
int	*env_builtin(t_mini *mini, t_cmd *cmd) // handles $NAME and prints its value eg. "Zuzka"
{
	int		i;
	char	*str;
	char	**env_arr;

	i = 0;
	(void)cmd;
	str = NULL;
	env_arr = mini->env;
	while (env_arr[i])
	{
		if (ft_strchr(env_arr[i], '='))
		{
			str = env_arr[i];
			write(1, env_arr[i], ft_strlen(env_arr[i]));
			write(1, "\n", 1);
		}
		env_arr++;
	}
	return (0);
}

char	**add_back_array(char **envs, char *new_env)
{
	int		i;
	int		j;
	char	**res;

	if (!envs || !(*envs) || !new_env)
		return (NULL);
	i = 0;
	while (envs[i])
		i++;
	res = malloc(sizeof(char *) * (i + 2));
	if (!res)
		return (NULL);
	j = 0;
	while (j < i)
	{
		res[j] = ft_strdup(envs[j]);
		if (!res[j])
		{
			free_char_pp(res);
			return (NULL);
		}
		j++;
	}
	res[i] = ft_strdup(new_env);
	if (!res[i])
	{
		free_char_pp(res);
		return (NULL);
	}
	res[i + 1] = NULL;
	return (res);
}

char *get_env_name(char *env)
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

int	exchange_env(char **envs, char *new_env, char *new_env_name)
{
	// TODO: UPDATE HEAD OF ARRAY IF WE ARE EXCHANGING TEH FIRST ELEMENT OF ARRAY ?
	int		i;
	char	*curr_env_name;
	//char	**second_part_of_arr;

	if (!envs || !(*envs) || !new_env)
		return (ERROR);
	i = 0;
	printf("exchanging\n");
	while (envs[i])
	{
		curr_env_name = get_env_name(envs[i]);
		if (!curr_env_name)
			return (ERROR);
		if (!ft_strncmp(curr_env_name, new_env_name, ft_strlen(curr_env_name)))
		{
			free(curr_env_name);
			curr_env_name = NULL;
			//second_part_of_arr = envs;
			//second_part_of_arr++;
			free(envs[i]);
			envs[i] = ft_strdup(new_env);
			if (!envs[i]) // Handle strdup failure
			{
				free_char_pp(envs);
				//free_char_pp(second_part_of_arr);
				return (ERROR);
			}
			return (0); // Successfully replaced the variable
		}
		free(curr_env_name);
		i++;
	}
	return (ERROR);
}

int	env_exists(char **envs, char *env_name)
{
	int		i;
	char	*curr_env_name;

	if (!envs || !(*envs) || !env_name)
		return (0);
	i = 0;
	while (envs[i])
	{
		curr_env_name = get_env_name(envs[i]);
		if (!curr_env_name)
			return (ERROR);
		if (!ft_strncmp(curr_env_name, env_name, ft_strlen(curr_env_name)))
		{
			free(curr_env_name);
			curr_env_name = NULL;
			return (1);
		}
		free(curr_env_name);
		curr_env_name = NULL;
		i++;
	}
	return (0);
}

// appends variable into mini->env array
int	export_builtin(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	**vars;
	char	**temp;
	char	*env_name;

	i = 1; // start at 1 because the 0th argument is the command itself
	temp = NULL;
	vars = cmd->args;
	while (vars[i])
	{
		env_name = get_env_name(vars[i]);
		if (!env_name)
			return (ERROR);
		if (env_exists(mini->env, env_name))
		{
			if (exchange_env(mini->env, vars[i], env_name) == ERROR)
			{
				free(env_name);
				env_name = NULL;
				return (ERROR);
			}
		}
		else
		{
			temp = add_back_array(mini->env, vars[i]);
			if (temp)
			{
				free_char_pp(mini->env);
				mini->env = temp;
			}
		}
		free(env_name);
		env_name = NULL;
		i++;
	}
	return (0);
}

// prints all arguments after it,
// int	echo_builtin(char *value)
// {


// }
