/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:34:16 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/23 18:43:02 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	int		i;
	char	*copy;

	i = 0;
	while (str[i] != '\0')
		i++;
	copy = (char *)malloc(sizeof(char) * (i + 1));
	i = 0;
	if (copy == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

// int	main(void)
// {
// 	char	str1[] = "hello world";
// 	printf("%s", ft_strdup(str1));
// 	//free(ft_strdup(str1));
// }