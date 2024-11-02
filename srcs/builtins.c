/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 09:51:55 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/02 22:18:46 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// changes directory, handles absolute, relative, no path
// call for type EXECUTABLE
// @returns 0 on successful change of directory, 1 on error
int	cd_builtin(char *path)
{
	if (path == NULL || !ft_strncmp(path, "~", 1))
	{
		char *home = getenv("HOME");
		chdir(home);
		free(path); // maybe will be done in caller function
		return (0);
	}
	// check if what we want to access is actually a directory and exists
	// if path starts with / searches absolute path, if not searches relative path
	if (is_directory(path))
	{
		chdir(path);
		free(path); // maybe will be done in caller function
		return (0);
	}
	else
		return (ERROR);
}

// duplicates pwd - WORKS
// call for type EXECUTABLE
// @returns 0 on success, 1 on error
int	pwd_builtin()
{
	char *cwd;
	int cwdlen;

	cwd = get_current_directory(); // testing finding a path when we will be expecting path type
	if (!cwd)
	{
		error_msg("error getting current directory");
		return (ERROR);
	}
	cwdlen = ft_strlen(cwd);
	write(1, cwd, cwdlen);
	write(1, "\n", 1);
	free(cwd);
	return (0);
}

// removes file from the system - if it is link removes the link, if it is unlinked, completely removes file from the filesystem - WORKS
// call for type EXECUTABLE
// @returns 0 on success, 1 on error (but somehow returns -1 always, no matter the return on error number)
int	unlink_builtin(char *path)
{
    if (unlink(path) == 0)
        return (0);
    else
        return (ERROR);
}
