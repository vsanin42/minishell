/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:38:24 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/14 19:26:52 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// finds cwd and stores it in malloced string, return must be freed - WORKS
// @returns malloced cwd string, NULL on failure
char	*get_current_directory(void)
{
	char	*cwd;

	cwd = (char*)malloc(PATH_MAX * sizeof(char)); // allocate memory for buffer where getcwd stores output
	if (!cwd)
		return (NULL);
	if (getcwd(cwd, PATH_MAX) == NULL)	// getcwd stores current working directory in buffer that we have to allocate
	{
		free(cwd); // free memory on failure
		return (NULL);
	}
	return (cwd);
}

// checks if file is directory - WORKS
// @returns >0 if is dir, 0 if not, -1 at error
int	is_directory(const char *path)
{
	struct stat	sb;

	// lstat automatically fills in the stat struct with information about the file/dir pointed to by path
	if (lstat(path, &sb) == -1) // fails when file doesn't exist or are insufficient permissions
	{
		error_msg("lstat failed", /* mini */NULL, NULL, NULL);
		return (-1);
	}
	// st_mode now stores permissions and file type
	// S_ISDITR is macro that checks if file we input as parameter is dir
	// returns non-zero if it's a directory, 0 if not
	return (S_ISDIR(sb.st_mode));
}

// checks if file is a regular file and if it is executable file
int is_executable_file(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
	{
		return (-1);
	}
	return (S_ISREG(sb.st_mode) && (sb.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))); // returns non-zero if it's regular file, 0 if not
}

// checks if file is a regular file and if it is readable
// if errno is ENOENT the file does not exist
int is_readable_file(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
	{
			return (-1);
	}
	return (S_ISREG(sb.st_mode) && (sb.st_mode & (S_IRUSR | S_IRGRP | S_IROTH)));
}

// checks if file is a regular file and if it is writable
int is_writable_file(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
	{
		return (-1);
	}
	return (S_ISREG(sb.st_mode) && (sb.st_mode & (S_IWUSR | S_IWGRP | S_IWOTH))); // returns non-zero if it's regular file, 0 if not
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
char	*get_path_env(char *cmd)
{
	char	**paths;
	char	*path;
	int		i;
	char	*path_without_cmd;
	char	*env_path;

	i = 0;
	env_path = getenv("PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path_without_cmd = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_without_cmd, cmd);
		free(path_without_cmd);
		if (!path)
			return (NULL);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (NULL);
}
