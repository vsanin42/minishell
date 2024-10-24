/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 12:40:19 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/21 19:20:00 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int	i;

	i = ft_strlen((char *)str);
	while (*str != '\0')
		str += 1;
	while (i >= 0)
	{
		if (*str == (char)c)
			return ((char *)str);
		str -= 1;
		i -= 1;
	}
	return (NULL);
}

// int	main() {
// 	printf("%s", ft_strrchr("tennessee", '\0'));
// }