/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:33:02 by vsanin            #+#    #+#             */
/*   Updated: 2024/07/26 18:49:16 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen_gnl(char const *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strchr_gnl(const char *str, int c)
{
	if (!str)
		return (NULL);
	while (*str != '\0')
	{
		if (*str == (char)c)
			return ((char *)str);
		str++;
	}
	if (*str == (char)c)
		return ((char *)str);
	return (NULL);
}

char	*ft_strjoin_gnl(char *buffer, char *line)
{
	size_t	len;
	char	*res;
	char	*res_orig;
	char	*buf;

	if (!buffer && !line)
		return (NULL);
	len = ft_strlen_gnl(line);
	if (buffer)
		len = ft_strlen_gnl(buffer) + ft_strlen_gnl(line);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	res_orig = res;
	buf = buffer;
	while (buffer && *buffer != '\0')
		*res++ = *buffer++;
	while (line && *line != '\0')
		*res++ = *line++;
	*res = '\0';
	free(buf);
	return (res_orig);
}
