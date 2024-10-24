/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 12:08:24 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/22 18:07:55 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*makesub(size_t len)
{
	char	*sub;

	sub = (char *)malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	return (sub);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*sub;
	char	*res;

	if (!s)
		return (NULL);
	if (start > (unsigned int)ft_strlen((char *)s))
		return (ft_strdup(""));
	if (start + len > (unsigned int)ft_strlen((char *)s))
		len = (unsigned int)ft_strlen((char *)s) - start;
	sub = makesub(len);
	if (!sub)
		return (NULL);
	res = sub;
	s += start;
	while (len-- && *s != '\0')
		*sub++ = *s++;
	*sub = '\0';
	return (res);
}

// int main()
// {
// 	printf("%s", ft_substr("", 5, 3));
// }