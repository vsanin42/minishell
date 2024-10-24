/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:21:10 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/23 18:22:22 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	int		i;
	int		j;

	i = 0;
	if (little[0] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && len--)
	{
		j = 0;
		while (big[i + j] == little[j])
		{
			if (little[j + 1] == '\0' && (size_t)j <= len)
				return ((char *)big + i);
			j++;
		}
		i++;
	}
	return (NULL);
}

// int main()
// {
// 	printf("%s", ft_strnstr("biba boba biba boba", "boba", 12));
// }