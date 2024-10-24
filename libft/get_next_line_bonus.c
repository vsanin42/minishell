/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:32:26 by vsanin            #+#    #+#             */
/*   Updated: 2024/10/03 11:48:16 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*cleanup(char *line, char *buffer)
{
	free(line);
	free(buffer);
	buffer = NULL;
	return (buffer);
}

static char	*process_line(char *buffer)
{
	int		len;
	int		i;
	char	*line;

	len = 0;
	i = 0;
	while (buffer[len] != '\n' && buffer[len] != '\0')
		len++;
	if (buffer[len] == '\n')
		len += 1;
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	while (i < len)
	{
		line[i] = buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*update_buffer(char *buffer)
{
	char	*new_buffer;
	char	*new_start;
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	if (*buffer == '\0')
		return (free(buffer), NULL);
	new_start = ft_strchr_gnl(buffer, '\n');
	if (new_start)
		len = ft_strlen_gnl(new_start + 1);
	new_buffer = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_buffer)
		return (NULL);
	while (i < len)
	{
		new_buffer[i] = new_start[i + 1];
		i++;
	}
	if (i == 0)
		return (free(buffer), free(new_buffer), NULL);
	new_buffer[i] = '\0';
	free(buffer);
	return (new_buffer);
}

int	check_early_exit(char **buffer, int fd)
{
	int	i;

	i = 0;
	if (fd == -1)
	{
		while (i < MAX_FD)
		{
			if (buffer[i])
			{
				free(buffer[i]);
				buffer[i] = NULL;
			}
			i++;
		}
		return (1);
	}
	return (0);
}

char	*get_next_line(int fd)
{
	char		*line;
	int			bytes_read;
	static char	*buffer[MAX_FD];

	bytes_read = 1;
	check_early_exit(buffer, fd);
	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	line = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!line)
		return (NULL);
	while (!(ft_strchr_gnl(buffer[fd], '\n')) && bytes_read != 0)
	{
		bytes_read = read(fd, line, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		line[bytes_read] = '\0';
		buffer[fd] = ft_strjoin_gnl(buffer[fd], line);
	}
	if (bytes_read < 0 || (!bytes_read && (!buffer[fd] || !*buffer[fd])))
		return (cleanup(line, buffer[fd]));
	free(line);
	line = process_line(buffer[fd]);
	buffer[fd] = update_buffer(buffer[fd]);
	return (line);
}
