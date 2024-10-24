/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 14:59:51 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/23 15:28:55 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (s && f)
	{
		while (*s != '\0')
		{
			f(i, s);
			s += 1;
			i += 1;
		}
	}
}

// void example_function(unsigned int index, char *c) {
//     // Add 1 to the character ASCII value
// 	(void)index;
//     (*c)++;
// }

// int main() {
//     char test_string[] = "Hello, world!";

//     printf("Original string: %s\n", test_string);

//     // Apply example_function to each character of the test_string
//     ft_striteri(test_string, &example_function);

//     printf("Modified string: %s\n", test_string);

//     return 0;
// }