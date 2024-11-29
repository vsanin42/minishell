/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zpiarova <zpiarova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:33:09 by vsanin            #+#    #+#             */
/*   Updated: 2024/11/29 10:56:45 by zpiarova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// based on received token finds its type
// if no token type is found, then it is text token
// @returns type of token based on enum defined in header
// @param value textual value of token
t_type	get_type(char *value)
{
	if (ft_strlen(value) == 2)
	{
		if (!ft_strncmp(value, ">>", 2))
			return (TOKEN_APPEND);
		if (!ft_strncmp(value, "<<", 2))
			return (TOKEN_HEREDOC);
	}
	else if (ft_strlen(value) == 1)
	{
		if (value[0] == '>')
			return (TOKEN_REDIROUT);
		if (value[0] == '<')
			return (TOKEN_REDIRIN);
		if (value[0] == '|')
			return (TOKEN_PIPE);
	}
	return (TOKEN_TEXT);
}

// creates new token from value and appends it to end of token list
// frees node_value it received, value of token is newly malloced
// expands envs and removes trailing quotes from start/end
// only if the previous token is not heredoc based on hdoc flag
// @returns 1 on success, 0 on error - BECAUSE OF NORM IN CALLER :D
int	create_and_add_tok(t_mini *mini, char *node_value, t_token **token_list, int *hdoc)
{
	char	*new_value;
	t_token	*new_tok;

	new_value = NULL;
	new_tok = new_token(ft_strdup(node_value), get_type(node_value));
	free(node_value);
	node_value = NULL;
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

// separates text characters(non-delimiters) from readline input into tokens
// called as soon as we encounter first text char that is not operator
// runs until we are still on text characters, until encounters a delimiter
// @param text input string
// @param i pointer to the current character, starts at first text character
// @param in_sq, in_dq are always called with 0 at start
// @returns created text node and moves iteration pointer to correct character
// when encounter '/", set flag to treat other delimiters as normal characters
// when encounter closing '/", set flag to treat delimiters as special chars
// when encounter delim., create node value from collected text and return it
// i is now at last el
// if normal text just move to the next char by i++
// at end decrease value to the last element of the collected string,
// because in outer function it will be increased again
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
char	*create_node_value(char *input, int *i)
{
	char	*node_value;

	node_value =  NULL;
	if (input[*i] == '\0')
	{
		//--(*i);
		return (NULL);
	}
	if ((input[*i] == '>' && input[*i + 1] == '>') || (input[*i] == '<'
			&& input[*i + 1] == '<'))
		node_value = ft_substr(input, (*i)++, 2);
	else if (input[*i] == '|' || input[*i] == '>' || input[*i] == '<')
		node_value = ft_substr(input, *i, 1);
	else
		node_value = process_text(input, i, 0, 0);
	return (node_value);
}

// i hate norminette
void init_gtl_vars(int *f, int *i, char **node, t_token **token)
{
	*f = 0;
	*i = -1;
	*node = NULL;
	*token = NULL;
}

// iterates over characters from strings received from readline
// if it is whitespace, just moves past it
// if it is one of operators, stores it as separate token with its type
// if it is text, stores it as text until encountering delimeter
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
			{
				free_token_list(token_list);
				token_list = NULL;
				return (NULL);
			}
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

	token_list = get_token_list(mini, input);
	free(input);
	input = NULL;
	if (!token_list)
		return (ERROR);
		//return (error_msg("Lexer error", mini, 0, 0));
	mini->token_list = token_list;
	return (0);
}
