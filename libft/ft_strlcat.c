/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 17:53:17 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/22 22:35:40 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_strlcat(char *dest, char const *src, unsigned int size)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	init_len;
	unsigned int	src_len;

	i = 0;
	init_len = ft_strlen(dest);
	src_len = ft_strlen((char *)src);
	j = init_len;
	if (size == 0 || size <= init_len)
		return (size + src_len);
	while (src[i] != '\0' && i < (size - init_len - 1))
	{
		dest[j] = src[i];
		i++;
		j++;
	}
	dest[j] = '\0';
	return (init_len + src_len);
}

// int main(void)
// {
// 	char str1[20] = "aaa";
// 	char *str2 = "bbb";
// 	printf("Length: %d\n", ft_strlcat(str1, str2, 6));
// 	printf("Result: %s\n", str1);
// }