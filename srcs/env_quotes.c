/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/11 19:59:35 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_q_or_end(char *text)
{
	int		i;
	char	q_start;

	i = 0;
	q_start = '\0';
	if (text[i] == '\'' || text[i] == '"')
		q_start = text[i++];
	while (text[i])
	{
		if (q_start)
		{
			if (text[i] == q_start)
				break ;
		}
		else
		{
			if (text[i] == '\'' || text[i] == '"')
				break ;
		}
		i++;
	}
	if (q_start && text[i] != '\0')
		i++;
	return (i);
}

int	find_words(char *text)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (text[i] != '\0')
	{
		while (text[i] == '\'' || text[i] == '"')
			i++;
		if (text[i] != '\0')
			count++;
		while (text[i] != '\0' && text[i] != '\'' && text[i] != '"')
			i++;
	}
	return (count);
}

char	**trim_quotes_in_array(char **head)
{
	char	**temp;
	char	*oldstr;

	temp = head;
	oldstr = NULL;
	while (temp && *temp)
	{
		if (*temp[0] == '\'')
		{
			oldstr = *temp;
			*temp = ft_strtrim(*temp, "'");
			if (oldstr)
				free(oldstr);
		}
		else if (*temp[0] == '"')
		{
			oldstr = *temp;
			*temp = ft_strtrim(*temp, "\"");
			if (oldstr)
				free(oldstr);
		}
		temp++;
	}
	return (head);
}

char	*str_from_array(char **head)
{
	char	*str;
	char	*res;
	char	**thead;
	char	*temp;

	thead = head;
	res = malloc(sizeof(char) * (array_char_len(thead) + 1));
	if (!res)
		return (NULL);
	str = res;
	while (thead && *thead)
	{
		temp = *thead;
		while (*temp)
		{
			*str = *temp;
			str++;
			temp++;
		}
		thead++;
	}
	*str = '\0';
	free_char_pp(head);
	return (res);
}

char 	**parse_eq(t_token *token)
{
	char	*text;
	int		i;
	char	**text_array;
	char	**head;

	i = 0;
	text = token->value;
	text_array = malloc(sizeof(char *) * (find_words(text) + 1));
	head = text_array;
	while (text[i])
	{
		if (text[i] == '"' || text[i] == '\'')
		{
			if (check_next_char(text[i], text[i + 1], i) > i)
			{
				i = check_next_char(text[i], text[i + 1], i);
				continue ;
			}
		}
		*text_array = exp_sub(ft_substr(text, i, find_q_or_end(text + i)));
		text_array++;
		i += find_q_or_end(text + i);
	}
	*text_array = NULL;
	return (trim_quotes_in_array(head));
}
