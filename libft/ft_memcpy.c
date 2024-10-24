/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:28:29 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/21 19:19:25 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	void	*dest_origin;

	dest_origin = dest;
	if (dest == NULL && src == NULL)
		return (dest);
	while (n > 0)
	{
		*(char *)dest++ = *(char *)src++;
		n--;
	}
	return (dest_origin);
}

// int main () {
//    const char src[50] = "https://www.tutorialspoint.com";
//    char dest[50];
//    strcpy(dest,"Heloooo!!");
//    printf("Before memcpy dest = %s\n", dest);
//    //ft_memcpy(dest, src, strlen(src)+1);
//    ft_memcpy(dest, src, 5);
//    printf("After memcpy dest = %s\n", dest);

//    return(0);
// }