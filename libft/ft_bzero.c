/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:17:02 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/21 19:18:36 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	while (n > 0)
	{
		*(unsigned char *)str++ = '\0';
		n--;
	}
}

// // add void* and return str in the func to test
// int main () {
// 	char	str[50];

// 	strcpy(str,"This is string.h library function");
// 	puts(str);

// 	ft_bzero(str,7);
// 	puts(str);

// 	return(0);
// }
