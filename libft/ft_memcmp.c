/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:57:33 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/23 18:27:27 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;

	a = (unsigned char *)s1;
	b = (unsigned char *)s2;
	while (n--)
	{
		if (*a != *b)
		{
			return (*a - *b);
		}
		a += 1;
		b += 1;
	}
	return (0);
}

// int	main()
// {
// 	char s[] = {-128, 0, 127, 0};
// 	//char sCpy[] = {-128, 0, 127, 0};
// 	char s2[] = {0, 0, 127, 0};
// 	char s3[] = {0, 0, 42, 0};
// 	// /* 1 */
// 	// //printf("Test 1: returned %d", ft_memcmp(s, sCpy, 4));
// 	// /* 2 */ check(!ft_memcmp(s, s2, 0)); showLeaks();
// 	// //printf("Test 2: returned %d", ft_memcmp(s, s2, 0));
// 	// /* 3 */ check(ft_memcmp(s, s2, 1) > 0); showLeaks();
// 	// /* 4 */ check(ft_memcmp(s2, s, 1) < 0); showLeaks();
// 	/* 5 */
// 	printf("Test 1 returned %d\n", ft_memcmp(s2, s3, 4));
// 	printf("Test 2 returned %d\n", ft_memcmp(s, s2, 1));
// 	// const void *s1 = "abc";
// 	// const void *s2 = "aab";
// 	// size_t n = 2;
// 	// printf("%d", ft_memcmp(s1, s2, n));
// }