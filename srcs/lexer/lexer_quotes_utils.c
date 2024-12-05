/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:22:25 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/04 21:52:43 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// we have array of strings created from text string separated based on "/'
// based on whether and which quote we have, we want or dont to expand envs
// @returns expanded string if allowed to expand or the string back if in ''
char	*exp_sub(t_mini *mini, char *str)
{
	char	*expanded;

	if (str[0] == '"' || str[0] != '\'')
	{
		expanded = get_env_value_to_process(mini, str);
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
	// if (c != c2 && c2 == '\0') -------------- checked in check_input(), at this point all quotes are surely paired
	// 	error_msg("\nError: unclosed quote", NULL, NULL, NULL); --- this also kinda pointless because the ERROR return goes nowhere
	return (i);
}
