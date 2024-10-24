/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:01:53 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/24 20:00:27 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*dest_origin;

	dest_origin = dest;
	if (dest == NULL && src == NULL)
		return (NULL);
	if (dest == src)
		return (dest);
	if (dest < src)
	{
		while (n--)
			*(char *)dest++ = *(char *)src++;
	}
	else
	{
		while (n--)
			((char *)dest)[n] = ((char *)src)[n];
	}
	return (dest_origin);
}
