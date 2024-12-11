/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:38:24 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/12/10 16:19:52 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// checks if file is directory
// @returns >0 if is dir, 0 if not, -1 at error
// lstat fills in stat struct with information about file/dir at path
// lstat fails when file doesn't exist or are insufficient permissions
// lstat also sets errno properly
// st_mode now stores permissions and file type
// S_ISDITR is macro that checks if file we input as parameter is dir
// returns non-zero if it's a directory, 0 if not
int	is_directory(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
		return (-1);
	return (S_ISDIR(sb.st_mode));
}

// checks if file is a regular file and if it is executable file
// @returns > 0 if it's regular file with x perm, 0 if not, -1 if not found
int	is_executable_file(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
		return (-1);
	return (S_ISREG(sb.st_mode)
		&& (sb.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)));
}

// checks if file is a regular file and if it is readable
// if errno is ENOENT the file does not exist
// @returns non-zero if it's regular readable file, 0 if not
int	is_readable_file(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
		return (-1);
	return (S_ISREG(sb.st_mode)
		&& (sb.st_mode & (S_IRUSR | S_IRGRP | S_IROTH)));
}

// checks if file is a regular file and if it is writable
// @returns non-zero if it's regular writable file, 0 if not
int	is_writable_file(const char *path)
{
	struct stat	sb;

	if (lstat(path, &sb) == -1)
		return (-1);
	return (S_ISREG(sb.st_mode)
		&& (sb.st_mode & (S_IWUSR | S_IWGRP | S_IWOTH)));
}
