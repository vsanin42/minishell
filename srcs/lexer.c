/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:33:09 by vsanin            #+#    #+#             */
/*   Updated: 2024/10/29 17:12:58 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_token(char *str, int *i, t_list **token_list)
{
	if (check_if_pipe(str, i, token_list))
		return (ERROR);
	return (0);
}

t_list	*get_token_list(char *input)
{
	t_list	*token_list;
	int		i;

	token_list = NULL;
	i = 0;
	while (input && input[i])
	{
		if (iswhitespace(input[i]))
		{
			i++;
			continue ;
		}
		if (check_token(input, &i, &token_list) == ERROR)
			break;
		i++;
	}
	return (token_list);
}

// t_token_type	get_token_type()
// {
	
// }

t_list	*lexer(char *input)
{
	t_list	*token_list;
	
	token_list = get_token_list(input);
	if (!token_list)
		return (NULL);
	return (token_list);
}