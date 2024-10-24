/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:14:26 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/22 19:08:15 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	part_of_set(char c, char const *set)
{
	while (*set != '\0')
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*trimmed;

	start = 0;
	end = ft_strlen((char *)s1);
	while (s1[start] && part_of_set(s1[start], set))
		start++;
	while (end > start && part_of_set(s1[end - 1], set))
		end--;
	trimmed = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!trimmed)
		return (NULL);
	ft_strlcpy(trimmed, (char *)&s1[start], end - start + 1);
	return (trimmed);
}

// int main()
// {
// 	char const *s1 = "";
// 	char const *set = "";
// 	char *result = ft_strtrim(s1, set);
// 	if (result)
//     {
// 		printf("Original string: '%s'\n", s1);
//         printf("Trimmed string: '%s'\n", result);
//         free(result);
//     }
// }