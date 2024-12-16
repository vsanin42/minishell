/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/12/16 04:35:45 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// no space for this in array.c :/
int	array_char_len(char **head)
{
	int	i;

	i = 0;
	while (head && *head)
	{
		i += ft_strlen(*head);
		head++;
	}
	return (i);
}

// finds how many words will be in array based on opening and closing quotes
// @returns number of found "words" delimited by quotes within a string
int	find_words(char *text)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (text[i] != '\0')
	{
		if (text[i] == '\'')
		{
			while (text[i] == '\'')
			i++;
		}
		else if (text[i] == '"')
		{
			while (text[i] == '"')
			i++;
		}
		if (text[i] != '\0')
			count++;
		while (text[i] != '\0' && text[i] != '\'' && text[i] != '"')
			i++;
	}
	return (count);
}

// trims all front and end quotes from each string in array of strings
// only trims quotes that are there as special characters, not quotes as text
// @returns array with trimmed quotes
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

// gets an array of strings and makes null-teminated string out of it
// frees the array and its element at end
// @returns collected string
char	*str_from_array(char **head)
{
	char	*str;
	char	*res;
	char	**thead;
	char	*temp;

	if (!head || !(*head))
		return (free_arr(head), NULL);
	thead = head;
	res = malloc(sizeof(char) * (array_char_len(thead) + 1));
	if (!res)
		return (NULL);
	str = res;
	while (thead && *thead)
	{
		temp = *thead;
		while (*temp)
			*str++ = *temp++;
		thead++;
	}
	*str = '\0';
	free_arr(head);
	return (res);
}

// in text token, removes unnecessary quotes and expands envs where needed
// @params token text token to edit
// separates text input based on quotes as delimiters where they are special
// check_next_char checks if we have ""/'', if yes move without saving in array
// we expand envs when found and store them as value of that array element
// @returns array of strings with each "word" having trailing quotes removed
char	**process_envs_and_quotes(t_mini *mini, t_token *token)
{
	int		i;
	char	*txt;
	char	**head;
	char	**text_array;

	i = 0;
	txt = token->value;
	text_array = malloc(sizeof(char *) * (find_words(txt) + 1));
	head = text_array;
	while (txt[i])
	{
		if (cnc_check(txt, &i) == 1)
			continue ;
		*text_array = exp_sub(mini, ft_substr(txt, i, find_q_or_end(txt + i)));
		if (*text_array == NULL)
			return (NULL);
		text_array++;
		i += find_q_or_end(txt + i);
	}
	*text_array = NULL;
	return (trim_quotes_in_array(head));
}
