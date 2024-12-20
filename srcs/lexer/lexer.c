/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:33:09 by vsanin            #+#    #+#             */
/*   Updated: 2024/12/16 19:32:27 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// creates new token from value and appends it to end of token list
// frees node_value string it received, value of token is newly malloced
// expands envs and removes trailing quotes from start/end
// only if the previous token is not heredoc based on hdoc flag
// @returns 1 on success, 0 on error - BECAUSE OF NORM IN CALLER :D
int	create_and_add_tok(t_mini *mini, char *val, t_token **token_list, int *hdoc)
{
	char	*new_value;
	t_token	*new_tok;

	new_value = NULL;
	new_tok = init_new_token(ft_strdup(val), get_token_type(val));
	free(val);
	val = NULL;
	if (!new_tok)
		return (0);
	if (new_tok->type == TOKEN_TEXT && *hdoc == 0)
	{
		new_value = str_from_array(process_envs_and_quotes(mini, new_tok));
		if (!new_value)
		{
			free(new_tok);
			return (0);
		}
		free(new_tok->value);
		new_tok->value = new_value;
	}
	if (new_tok->value)
		add_back_token(token_list, new_tok);
	*hdoc = 0;
	return (1);
}

// separates text characters(non-delimiters) from input into text tokens
// called as soon as we encounter first text char that is not operator
// runs until we are still on text characters, until encounters a delimiter
// @param text input string
// @param i pointer to the current character, starts at first text character
// @param in_sq, in_dq are always called with 0 at start
// @returns created text node and moves iteration pointer to correct character
// when encounter '/", set flag to treat other delimiters as normal characters
// when encounter closing '/", set flag to treat delimiters as special chars
// when encounter delim., create node value from collected text and return it
// i is now at last el of created string from text
// if normal text just move to the next char by i++
// at end decrease value to the last element of the collected string,
// because in outer function it will be increased again
// @returns text string from start of text until encountering delimeter
char	*process_text(char *text, int *i, int in_sq, int in_dq)
{
	char	*node_value;
	int		start;

	start = *i;
	node_value = NULL;
	while (text && text[(*i)])
	{
		if (text[*i] == '"' && in_sq == 0)
			in_dq = !in_dq;
		else if (text[*i] == '\'' && in_dq == 0)
			in_sq = !in_sq;
		else if (!in_sq && !in_dq && (text[*i] == '|' || text[*i] == '>'
				|| text[*i] == '<' || iswhitespace(text[*i])))
		{
			(*i)--;
			node_value = ft_substr(text, start, *i - start + 1);
		}
		else if ((in_dq && text[*i] == '"') || (in_sq && (text[*i] == '\'')))
			node_value = ft_substr(text, start, *i - start + 1);
		if (node_value)
			return (node_value);
		(*i)++;
	}
	(*i)--;
	return (ft_substr(text, start, ft_strlen(text + start)));
}

// chunk of get_token_list that gets the value of a node
// if it is whitespace, just moves past it
// if it is one of operators, stores it as separate token with its type
// if its text, calls process_text function
// @returns created node value as string, caller creates token from it+appends
char	*create_node_value(char *input, int *i)
{
	char	*node_value;

	node_value = NULL;
	if (input[*i] == '\0')
		return (NULL);
	if ((input[*i] == '>' && input[*i + 1] == '>') || (input[*i] == '<'
			&& input[*i + 1] == '<'))
		node_value = ft_substr(input, (*i)++, 2);
	else if (input[*i] == '|' || input[*i] == '>' || input[*i] == '<')
		node_value = ft_substr(input, *i, 1);
	else
		node_value = process_text(input, i, 0, 0);
	return (node_value);
}

// iterates over characters from string received from readline
// calls get nodevalue that creates different types of tokens
// sets flag for the next iteration to indicate that heredoc was just processed
// and therefore don't enter the expanding/trimming functions
// stores received value as tokens value and appends it to end of token list
// @returns head of token_list
t_token	*get_token_list(t_mini *mini, char *input)
{
	int		hdoc_flag;
	int		i;
	char	*node_value;
	t_token	*token_list;

	init_gtl_vars(&hdoc_flag, &i, &node_value, &token_list);
	while (input[++i])
	{
		while (input[i] && iswhitespace(input[i]))
			i++;
		node_value = create_node_value(input, &i);
		if (node_value)
		{
			if (!create_and_add_tok(mini, node_value, &token_list, &hdoc_flag))
				return (free_token_list(mini), NULL);
			if (i + 1 < ft_strlen(input) && !ft_strncmp(input + i, "<<", 2))
			{
				hdoc_flag = 1;
				i++;
			}
		}
		else
			return (NULL);
	}
	return (token_list);
}

// creates list of all tokens in order from the input string from readline
int	lexer(char *input, t_mini *mini)
{
	t_token	*token_list;
	char	*input_trimmed_whitespaces;

	input_trimmed_whitespaces = ft_strtrim(input, " \t\n\r\v\f");
	free(input);
	input = NULL;
	if (!input_trimmed_whitespaces)
		return (ERROR);
	token_list = get_token_list(mini, input_trimmed_whitespaces);
	free(input_trimmed_whitespaces);
	input_trimmed_whitespaces = NULL;
	if (!token_list)
		return (ERROR);
	mini->token_list = token_list;
	return (0);
}
