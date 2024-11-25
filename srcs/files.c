/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:38:24 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/11/25 19:45:24 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// checks if file is directory - WORKS
// @returns >0 if is dir, 0 if not, -1 at error
int	is_directory(const char *path)
{
	struct stat	sb;

	// lstat automatically fills in the stat struct with information about the file/dir pointed to by path
	if (lstat(path, &sb) == -1) // fails when file doesn't exist or are insufficient permissions
	{
		return (-1);
	}
	// st_mode now stores permissions and file type
	// S_ISDITR is macro that checks if file we input as parameter is dir
	// returns non-zero if it's a directory, 0 if not
	return (S_ISDIR(sb.st_mode));
}

// checks if file is a regular file and if it is executable file
// @returns > 0 if it's regular file with execute permission, 0 if not, -1 if not found
int is_executable_file(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
	{
		return (-1);
	}
	return (S_ISREG(sb.st_mode) && (sb.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)));
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
