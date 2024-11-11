/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_v.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:22:25 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/11 14:52:37 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	array_char_len(char **head)
{
	int		i;

	i = 0;
	while (head && *head)
	{
		i += ft_strlen(*head);
		head++;
	}
	return (i);
}

char	*exp_sub(char *str)
{
	char *expanded;

	if (str[0] == '"' || str[0] != '\'')
	{
		expanded = get_env_value_to_process(str);
		free(str);
		str = NULL;
	}
	else
		expanded = str;
	return (expanded);
}

int	check_next_char(char c, char c2, int i)
{
	if (c == c2)
		i += 2;
	if (c != c2 && c2 == '\0')
		error_msg("\nError: unclosed quote", NULL, NULL, NULL);
	return (i);
}
