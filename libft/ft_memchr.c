/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:33:25 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/21 19:19:19 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	while (n)
	{
		if (*(unsigned char *)str == (unsigned char)c)
			return ((unsigned char *)str);
		str++;
		n--;
	}
	return (NULL);
}

// int	main()
// {
// 	printf("%s", (char *)ft_memchr("xyzxyzAxyz", 'A', 6));
// }