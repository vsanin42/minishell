/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:07:05 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/22 18:08:41 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	char	*res;
	char	*res_orig;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	res_orig = res;
	while (*s1 != '\0')
		*res++ = *s1++;
	while (*s2 != '\0')
		*res++ = *s2++;
	*res = '\0';
	return (res_orig);
}

// int main()
// {	
// 	char const *str1 = "Biba";
// 	char const *str2 = "Boba";
// 	char *result;

// 	result = ft_strjoin(str1, str2);
// 	printf("Result: %s\n", result);
// }