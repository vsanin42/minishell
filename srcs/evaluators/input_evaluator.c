/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_evaluator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:32:23 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/18 13:00:53 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_quotes(char *input)
{
	int	in_sq;
	int	in_dq;

	in_sq = 0;
	in_dq = 0;
	while (*input)
	{
		if (*input == '"' && in_sq == 0)
			in_dq = !in_dq;
		else if (*input == '\'' && in_dq == 0)
			in_sq = !in_sq;
		input++;
	}
	if (in_sq || in_dq)
		return (ERROR);
	return (0);
}

int	check_curly_braces(char *input)
{
	int	i;
	int	brace_count;

	brace_count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '{')
			brace_count++;
		else if (input[i] == '}' && brace_count > 0)
			brace_count--;
		i++;
	}
	if (brace_count != 0)
		return (ERROR);
	return (0);
}

int	check_bad_substitution(char *input, int i)
{
	if ((input[i] >= '0' && input[i] <= '9')
		|| (!ft_isalnum(input[i]) && input[i] != '_' && input[i] != '?'))
		return (ERROR);
	if (input[i] == '?' && input[i + 1] != '}')
		return (ERROR);
	else
		i++;
	while (input[i] && input[i] != '}')
	{
		if (!ft_isalnum(input[i]) && input[i] != '_')
			return (ERROR);
		i++;
	}
	return (0);
}

int	check_bs_base(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '{')
		{
			i += 2;
			if (check_bad_substitution(input, i) == ERROR)
				return (ERROR);
		}
		i++;
	}
	return (0);
}

int	check_input(char *input, t_mini *mini)
{
	if (check_quotes(input) == ERROR)
	{
		mini->exit_status = 1;
		return (s_error_msg("minishell: unclosed quote"), 1);
	}
	if (check_curly_braces(input) == ERROR)
	{
		mini->exit_status = 1;
		return (s_error_msg("minishell: unclosed brace"), 1);
	}
	if (check_bs_base(input) == ERROR)
	{
		mini->exit_status = 1;
		return (s_error_msg("minishell: bad substitution"), 1);
	}
	return (0);
}
