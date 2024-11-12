/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:22:25 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/12 23:01:17 by zpiarova         ###   ########.fr       */
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

// we have array created from etxt string separated based on "/'
// based on whether and which quote we have, we want or dont to expand envs
// @returns expanded string if allowed to expand or the string back if in ''
char	*exp_sub(char *str)
{
	char	*expanded;

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

// checks if the next character is the same as current character
// @returns index by which we should move in input string - +2 or as it was
int	check_next_char(char c, char c2, int i)
{
	if (c == c2)
		i += 2;
	if (c != c2 && c2 == '\0')
		error_msg("\nError: unclosed quote", NULL, NULL, NULL);
	return (i);
}
