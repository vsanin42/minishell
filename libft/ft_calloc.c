/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:52:39 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/24 14:31:00 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*buffer;

	if (num == 0 || size == 0)
		return (malloc(1));
	if (size != 0 && num > (size_t)(18446744073709551615U / size))
		return (NULL);
	buffer = malloc(num * size);
	if (!buffer)
		return (NULL);
	ft_bzero(buffer, num * size);
	return (buffer);
}

// int main() {
//     size_t num = 5;
//     size_t size = sizeof(int);

//     // allocating memory for an array of 5 integers
//     int *arr = (int *)ft_calloc(num, size);
//     if (arr == NULL) {
//         printf("Memory allocation failed\n");
//         return 1;
//     }

//     // test memory initialization to zero
//     for (size_t i = 0; i < num; i++) {
//         if (arr[i] != 0) {
//             printf("Memory not initialized to zero\n");
//             free(arr);
//             return 1;
//         }
//     }
//     printf("Memory successfully allocated and initialized to zero\n");

//     // test overflow
//     num = 18446744073709551615UL;
//     size = 2;
//     void *overflow_test = ft_calloc(num, size);
//     if (overflow_test == NULL) {
//         printf("Overflow correctly handled\n");
//     } else {
//         printf("Overflow not handled correctly\n");
//         free(overflow_test);
//         return 1;
//     }
//     free(arr);
//     return (0);
// }
