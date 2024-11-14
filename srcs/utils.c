/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:26:30 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/14 16:55:34 by zpiarova         ###   ########.fr       */
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

// receives string value WITHOUT $ that should be checked if it exists as env
// @returns expanded allocated value if found, NULL if not found,
// or allocated "$" if no value to check
char	*process_env(char *name)
{
	char	*res;

	if (!name)
		res = ft_strdup("$");
	else if (getenv(name))
	{
		res = ft_strdup(getenv(name));
		if (!res)
			return (NULL);
	}
	else
		res = NULL;
	return (res);
}

// counts text tokens in token list after input token and before end or pipe
// @returns number of text tokens after token from parameter
int	get_ttokens_len(t_token	*token)
{
	int		i;
	t_token	*temp;

	i = 0;
	temp = token;
	while (temp && temp->type != TOKEN_PIPE)
	{
		if (temp->type == TOKEN_TEXT)
			i++;
		temp = temp->next;
	}
	return (i);
}
