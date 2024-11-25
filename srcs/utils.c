/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:26:30 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/25 20:03:20 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// checks if the char is a whitespace
// @returns 0 if not whitespace, 1 if whitespace
int	iswhitespace(char c)
{
	if (c && (c == ' ' || c == '\t' || c == '\n'
			|| c == '\r' || c == '\v' || c == '\f'))
		return (1);
	return (0);
}

// checks if string consists of alphanumeric characters only
// @returns 1 if string is alnum, 0 if not
int	is_alnum(char *str)
{
	char	*temp;

	temp = str;
	while (*temp)
	{
		if (!ft_isalnum(*temp))
			return (0);
		temp++;
	}
	return (1);
}

// performs two strjoins: s1 and s2, then the result string and \n
// of two arguments frees ONLY S1 - to avoid using oldres and similar things
// s2 is input and is freed later in the caller function
// @returns: complete string with an appended new part and a newline
char	*str_append_nl(char *s1, char *s2)
{
	char	*tmp;
	char	*res;

	if (!s1 || !s2)
		return (NULL);
	tmp = ft_strjoin(s1, s2);
	free(s1);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, "\n");
	free(tmp);
	if (!res)
		return (NULL);
	return (res);
}
