/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_evaluator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:32:23 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/05 01:20:08 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_bad_substitution(char *input, int i)
{
	while (input[i] && (input[i] != '$' && input[i + 1] != '{'))
		i++;
	if (input[i] != '\0')
	{
		i += 2;
		if (input[i] >= '0' && input[i] <= '9')
			return (ERROR);
		while (input[i] && input[i] != '}')
		{
			if (!ft_isalnum(input[i]) && input[i] != '_'
				&& (input[i] == '?' && input[i + 1] != '}'
				&& input[i - 1] != '{'))
				return (ERROR);
			i++;
		}
	}
	return (0);
}

int check_braces_alnum(char *input, int start)
{
	while (input[start] && input[start] != '}')
	{
		if (!ft_isalnum(input[start]))
			return (2);
		start++;
	}
	return (0);
}

int	check_curly_braces(char *input)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '{')
		{
			j = i + 1;
			while (input[j])
			{
				if (input[j] == '}')
					return (2);//(check_braces_alnum(input, i + 2));
				j++;
			}
		}
		i++;
	}
	return (ERROR);
}

int	check_next_quote(char *input, int i)
{
	char	c;

	c = input[i];
	i++;
	while (input[i])
	{
		if (input[i] == c)
			return (i);
		i++;
	}
	return (0);

}

int	check_quotes(char *input)
{
	int	i;
	//int	j;
	int	q_count;

	i = 0;
	q_count = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			if (check_next_quote(input, i) == 0)
				return (1);
			i = check_next_quote(input, i);
		}
		i++;
	}
	return (q_count);
}

int	isbq(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '{')
			return (1);
		if (input[i] == '\'' || input[i] == '"')
			return (2);
		i++;
	}
	return (0);
}

int	check_input(char *input)
{
	if (isbq(input) == 1)
	{
		if (check_curly_braces(input) == 1)
			return (s_error_msg("Error: unclosed brace"), 1);
		else if (check_quotes(input) == 1)
			return (s_error_msg("Error: unclosed quote"), 1);
	}
	if (isbq(input) == 2)
	{
		if (check_curly_braces(input) == 2)
			return (s_error_msg("Error: non-alphanumeric char between {}"), 1);
		if (check_quotes(input) == 1)
			return (s_error_msg("Error: unclosed quote"), 1);
	}
	if (check_bad_substitution(input, 0) == ERROR)
		return (s_error_msg("Error: bad substitution"), 1);
	return (0);
}
