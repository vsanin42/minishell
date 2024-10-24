/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:12:37 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/21 19:19:53 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	int				ret;

	i = 0;
	ret = 0;
	while ((s1[i] || s2[i]) && n != i)
	{
		if (s1[i] != s2[i])
		{
			ret = s1[i] - s2[i];
			return (ret);
		}
		i++;
	}
	return (0);
}

// int	main()
// {
// 	char *s1 = "abc";
// 	char *s2 = "abb";
// 	size_t n = 2;
// 	printf("%d", ft_strncmp(s1, s2, n));
// }
