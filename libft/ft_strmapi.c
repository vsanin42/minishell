/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:49:04 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/23 15:11:42 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;

	i = 0;
	if (!s || !f)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	while (s[i] != '\0')
	{
		str[i] = f(i, s[i]);
		i += 1;
	}
	str[i] = '\0';
	return (str);
}

// char to_uppercase(unsigned int i, char c) {
//     (void)i;
//     if (c >= 'a' && c <= 'z') {
//         return c - 32;
//     }
//     return c;
// }

// int main() {
//     char *s = "hello, world!";
//     char *result = ft_strmapi(s, to_uppercase);

//     if (result) {
//         printf("Original: %s\n", s);
//         printf("Modified: %s\n", result);
//         free(result);
//     } else {
//         printf("Memory allocation failed\n");
//     }

//     return 0;
// }