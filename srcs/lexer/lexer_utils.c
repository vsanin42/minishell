/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 14:03:13 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/16 13:16:59 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// i hate norminette - :D :D :D :D -Zuzka
void	init_gtl_vars(int *f, int *i, char **node, t_token **token)
{
	*f = 0;
	*i = -1;
	*node = NULL;
	*token = NULL;
}

int	cnc_check(char *text, int *i)
{
	if (text[*i] == '"' || text[*i] == '\'')
	{
		if (check_next_char(text[*i], text[*i + 1], *i) > *i)
		{
			*i = check_next_char(text[*i], text[*i + 1], *i);
			return (1);
		}
	}
	return (0);
}

// finds end of the substring of our text token based on delimeters
// delimeter can be quotes or end of string
// q_ign = quote ignore flag used to skip past the next quote we find after $'
// breaks the loop if helper returns 1 - found a quote or \0
// increments past $ AND '/" if helper returns 2 - found a $ + '\" combo
// that means that the next quote is to be ignored because it must remain
// and be included in the whole string
// @returns index of the delimeter in the input string
int	find_q_or_end(char *text)
{
	int		i;
	char	q_start;
	int		q_ign;

	q_ign = 0;
	i = 0;
	q_start = '\0';
	if (text[i] == '\'' || text[i] == '"')
		q_start = text[i++];
	while (text[i])
	{
		if (find_q_helper(q_start, text, i, &q_ign) == 1)
			break ;
		else if (find_q_helper(q_start, text, i, &q_ign) == 2)
			i++;
		i++;
	}
	// not sure if q_start is important here but i havent seen any difference
	// NVM IT IS IMPORTANT XXDDDDDD
	if (q_start && text[i] != '\0')
		i++;
	return (i);
}

void	set_q_ign(int *q_ign, int value)
{
	*q_ign = value;
}

int	find_q_helper(char q_start, char *text, int i, int *q_ign)
{
	if (q_start)
	{
		if (text[i] == q_start && !*q_ign)
			return (1);
		else
			return (set_q_ign(q_ign, 0), 0);
	}
	else if (i != 0 && text[i - 1] && text[i - 1] == '$'
		&& (text[i] == '\'' || text[i] == '"'))
	{
		*q_ign = 1;
		return (2);
	}
	else
	{
		if ((text[i] == '\'' || text[i] == '"') && !*q_ign)
			return (1);
		else
			return (set_q_ign(q_ign, 0), 0);
	}
	return (0);
}
