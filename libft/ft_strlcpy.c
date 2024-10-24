/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 17:48:00 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/23 18:10:48 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_strlcpy(char *dest, char const *src, unsigned int size)
{
	unsigned int	i;
	unsigned int	len;

	i = 0;
	len = 0;
	while (src[len])
	{
		len++;
	}
	if (size == 0)
		return (ft_strlen(src));
	if (size != 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
	}
	dest[i] = '\0';
	return (len);
}

// int main()
// {
// 	char dest[10];
// 	char src[] = "1234567890";

// 	unsigned int len = ft_strlcpy(dest, src, 3);
// 	printf("%d", len);
// 	printf("%s", ", ");
// 	printf("%s", dest);
// 	return(0);
// }