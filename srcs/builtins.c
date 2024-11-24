/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/24 21:11:03 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// all builtins return 0 on success and 1 on error
// they do not quit the program in error, mut be handled by caller function
// TODO: must update exit status at end of each of these functions

// changes directory, handles absolute, relative, no path
// call for type EXECUTABLE
// FREE is done in caller function
// @returns 0 on successful change of directory, 1 on error
int	cd_builtin(t_cmd *cmd)
{
	char	*home;
	char	*path;

	path = cmd->args[1];
	if (path == NULL || !ft_strncmp(path, "~", 1))
	{
		home = getenv("HOME");
		if (!home)
		{
			perror("cd: HOME not set\n");
			return (ERROR); // Shell convention: return 1 for error
		}
		chdir(home);
		return (0);
	}
	if (get_args_len(cmd) == 3)
	{
		perror("minishell: string not in pwd: ");
		return (ERROR);
	}
	if (get_args_len(cmd) > 3)
	{
		perror("cd: too many arguments");
		return (ERROR);
	}
	if (is_directory(path) != 0)
	{
		perror("cd: not a directory: ");
		perror(path);
		return (ERROR);
	}
	if (chdir(path) != 0)
	{
		perror("cd: error changing directory");
		return (ERROR);
	}
	return (ERROR);
}

// duplicates pwd - WORKS
// call for type EXECUTABLE
// @returns 0 on success, 1 on error
int	pwd_builtin(t_cmd *cmd)
{
	char	*cwd;
	int		cwdlen;

	if (get_args_len(cmd) > 1)
	{
		perror("pwd: too many arguments");
		return (2);
	}
	cwd = get_current_directory(); // testing finding a path when we will be expecting path type
	if (!cwd)
		return (ERROR);
	cwdlen = ft_strlen(cwd);
	write(1, cwd, cwdlen);
	write(1, "\n", 1);
	free(cwd); // maybe will be don ein calling function
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
char	*env_builtin(char *name) // handles $NAME and prints its value eg. "Zuzka"
{
	char	*value;

	value = getenv(name);
	if (!value)
		return ("");
	return (value);
}

// prints all arguments after it,
// int	echo_builtin(char *value)
// {


// }
