/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:22:25 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/18 13:49:30 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**peaq_help(void)
{
	char	**text_array;

	text_array = malloc(sizeof(char *) * 2);
	if (!text_array)
		return (NULL);
	text_array[0] = ft_strdup(" ");
	text_array[1] = NULL;
	return (text_array);
}

int	check_dollar_sq(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '$' && str[i + 1] == '\'')
		i += 2;
	else
		return (0);
	while (str[i])
	{
		if (str[i] == '\'' && str[i + 1] == '\0')
			return (1);
		i++;
	}
	return (0);
}

char	*strdup_from_second(const char *str)
{
	int		i;
	char	*copy;

	i = 0;
	if (str == NULL)
		return (NULL);
	while (str[i] != '\0')
		i++;
	copy = (char *)malloc(sizeof(char) * i);
	i = 0;
	if (copy == NULL)
		return (NULL);
	while (str[i + 1] != '\0')
	{
		copy[i] = str[i + 1];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

// we have array of strings created from text string separated based on "/'
// based on whether and which quote we have, we want or dont to expand envs
// @returns expanded string if allowed to expand or the string back if in ''
char	*exp_sub(t_mini *mini, char *str)
{
	char	*expanded;

	if (str[0] != '\'' && !check_dollar_sq(str))
	{
		if (!ft_strncmp(str, "\"$\"\0", 4))
			expanded = str;
		else
		{
			expanded = get_env_value_to_process(mini, str);
			free(str);
			str = NULL;
		}
	}
	else if (check_dollar_sq(str))
	{
		expanded = strdup_from_second(str);
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
	return (i);
}
