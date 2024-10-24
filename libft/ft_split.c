/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 18:04:17 by vsanin            #+#    #+#             */
/*   Updated: 2024/05/23 20:15:56 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	words_len(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		while (str[i] == c)
			i++;
		if (str[i] != '\0')
			count++;
		while (str[i] != '\0' && str[i] != c)
			i++;
	}
	return (count);
}

static char	*ft_strndub(char *src, int n)
{
	char	*res;
	int		i;

	i = 0;
	res = (char *)malloc((n + 1) * sizeof(char));
	if (!res)
		return (NULL);
	while (i < n)
	{
		res[i] = src[i];
		i += 1;
	}
	res[i] = '\0';
	return (res);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		word_len;
	char	**split;
	int		current_word;

	if (!s)
		return (NULL);
	split = (char **)malloc(sizeof(char *) * (words_len((char *)s, c) + 1));
	if (!split)
		return (NULL);
	current_word = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		i = 0;
		while (s[i] != '\0' && s[i] != c)
			i++;
		word_len = i;
		if (word_len > 0)
			split[current_word++] = ft_strndub((char *)s, word_len);
		s += word_len;
	}
	split[current_word] = NULL;
	return (split);
}

// void	ft_putstr(char *str)
// {
// 	int	i;
// 	i = 0;
// 	while (str[i] != '\0')
// 	{
// 		write(1, &str[i], 1);
// 		i++;
// 	}
// }
// int	main(void)
// {
// 	int	i = 0;
// 	char *str = "??b";
// 	char c = '?';
// 	char **split = ft_split(str, c);
// 	while (split[i])
// 	{
// 		ft_putstr(split[i]);
// 		//write(1, "\n", 1);
// 		i++;
// 	}
// 	//printf("%d", words_len("?aaa!!!bbb?ccc!", "?!"));
// }