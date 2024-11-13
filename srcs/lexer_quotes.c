/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:57:06 by zpiarova          #+#    #+#             */
/*   Updated: 2024/11/12 23:00:09 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// finds end of the substring of our text token based on delimeters
// delimeter can be quotes or end of string
// @returns index of the delimeter in the input string
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

	if (!head || !(*head)) // added this, now doesnt pass norm, check pls if needed, if sure we do not need can remove
		return (NULL);
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

// in text token, removes unnecessary quotes and expands envs where needed
// separates text input based on quotes as delimiters where they are special
// check_next_char checks if we have ""/'', if yes move without saving in array
// we expand envs when found and store them as value of that array element
// @returns array of strings with each "word" having trailing quotes removed
// @params token text token to edit
char 	**process_envs_and_quotes(t_token *token)
{
	int		i;
	char	*text;
	char	**head;
	char	**text_array;

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
		// somewhere here we should not add to array if the value we get from exp_sub is NULL but cannot figure out where
		i += find_q_or_end(text + i);
	}
	*text_array = NULL; // and if we leave out that element of array fill the empty spaces from the back with NULLs
	return (trim_quotes_in_array(head));
}
